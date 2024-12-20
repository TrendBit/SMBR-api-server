#include "MyController.hpp"

MyController::MyController(const std::shared_ptr<oatpp::web::mime::ContentMappers>& apiContentMappers,
                           boost::asio::io_context& ioContext,
                           SystemModule& systemModule,
                           CommonModule& commonModule,
                           ControlModule& controlModule,
                           CanRequestManager& canRequestManager)
    : oatpp::web::server::api::ApiController(apiContentMappers)
    , m_ioContext(ioContext)
    , m_systemModule(systemModule)
    , m_commonModule(commonModule)
    , m_controlModule(controlModule)
    , m_canRequestManager(canRequestManager) {}

  // ==========================================
  // System Endpoints
  // ==========================================

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getSystemModules() {
    auto dtoList = oatpp::List<oatpp::Object<MyModuleInfoDto>>::createShared();
    std::promise<oatpp::List<oatpp::Object<MyModuleInfoDto>>> promise;
    auto future = promise.get_future();

    m_systemModule.getAvailableModules([&promise, dtoList](const std::vector<CanMessage>& responses) {
    for (const auto& response : responses) {
        App_messages::Common::Probe_modules_response moduleResponse;

        auto dataCopy = response.getData();
        if (moduleResponse.Interpret_data(dataCopy)) {
            auto moduleInfoDto = MyModuleInfoDto::createShared();

            std::stringstream uidHex;
            uidHex << "0x";
            for (const auto& byte : moduleResponse.uid) {
                uidHex << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte);
            }
            moduleInfoDto->uid = uidHex.str();

            uint32_t modulePart = (response.getId() >> 4) & 0xFF;
            switch (static_cast<Codes::Module>(modulePart)) {
                case Codes::Module::Core_module:
                    moduleInfoDto->module_type = "core";
                    break;
                case Codes::Module::Control_module:
                    moduleInfoDto->module_type = "control";
                    break;
                case Codes::Module::Sensor_module:
                    moduleInfoDto->module_type = "sensor";
                    break;
                default:
                    moduleInfoDto->module_type = "unknown";
                    break;
            }

            dtoList->push_back(moduleInfoDto);
        }
    }
        promise.set_value(dtoList);
    });


    future.wait();
    auto result = future.get();

    if (result->empty()) {
        return createResponse(Status::CODE_504, "No module responses received (timeout)");
    }

    return createDtoResponse(Status::CODE_200, result);
}


  // ==========================================
  // Common Endpoints
  // ==========================================

uint8_t MyController::getNextSeqNumber() {
    uint8_t currentSeq = m_seqNum.fetch_add(1);
    if (currentSeq == 255) {
        m_seqNum = 0;  
    }
    return currentSeq;
}

std::optional<Codes::Module> MyController::getTargetModule(const oatpp::Enum<dto::ModuleEnum>::AsString& module) {
    if (module == dto::ModuleEnum::control) {
        return Codes::Module::Control_module;
    } else if (module == dto::ModuleEnum::sensor) {
        return Codes::Module::Sensor_module;
    } else if (module == dto::ModuleEnum::core) {
        return Codes::Module::Core_module;
    }
    return std::nullopt;
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::ping(const oatpp::Enum<dto::ModuleEnum>::AsString& module) {
    auto pingResponseDto = MyPingResponseDto::createShared();
    std::promise<float> promise;
    auto future = promise.get_future();

    std::atomic<bool> promiseSet = false;
    auto handlePingResult = [&promise, &promiseSet](float responseTime) {
        if (!promiseSet.exchange(true)) {
            promise.set_value(responseTime);
        } 
    };

    auto targetModuleOpt = getTargetModule(module);  
    if (!targetModuleOpt.has_value()) {
        return createResponse(Status::CODE_404, "Module not found");  
    }
    Codes::Module targetModule = targetModuleOpt.value();

    uint8_t seq_num = getNextSeqNumber();

    m_commonModule.ping(m_canRequestManager, targetModule, seq_num, handlePingResult);

    future.wait();
    float responseTime = future.get();

    if (responseTime >= 0) {
        pingResponseDto->time_ms = responseTime;
        return createDtoResponse(Status::CODE_200, pingResponseDto);  
    } else if (responseTime == -2) {
        return createResponse(Status::CODE_504, "Ping timed out"); 
    } else {
        return createResponse(Status::CODE_500, "Ping failed"); 
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getCoreLoad(const oatpp::Enum<dto::ModuleEnum>::AsString& module) {
    auto loadResponseDto = MyLoadResponseDto::createShared();
    std::promise<float> promise;
    auto future = promise.get_future();

    auto handleLoadResult = [&promise](float load) {
        promise.set_value(load);
    };

    auto targetModuleOpt = getTargetModule(module);  
    if (!targetModuleOpt.has_value()) {
        return createResponse(Status::CODE_404, "Module not found");  
    }
    Codes::Module targetModule = targetModuleOpt.value();

    m_commonModule.getCoreLoad(m_canRequestManager, targetModule, handleLoadResult);

    future.wait();
    float load = future.get();

    if (load >= 0 && load <= 100) {
        loadResponseDto->load = load;
        return createDtoResponse(Status::CODE_200, loadResponseDto);
    } else if (load == -2) {
        return createResponse(Status::CODE_504, "Request timed out");
    } else {
        return createResponse(Status::CODE_500, "Failed to retrieve load");
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getCoreTemp(const oatpp::Enum<dto::ModuleEnum>::AsString& module) {
    auto tempResponseDto = MyTempDto::createShared();
    std::promise<float> promise;
    auto future = promise.get_future();

    auto handleTempResult = [&promise](float temperature) {
        promise.set_value(temperature);
    };

    auto targetModuleOpt = getTargetModule(module);  
    if (!targetModuleOpt.has_value()) {
        return createResponse(Status::CODE_404, "Module not found");  
    }
    Codes::Module targetModule = targetModuleOpt.value();

    m_commonModule.getCoreTemp(m_canRequestManager, targetModule, handleTempResult);

    future.wait();
    float temperature = future.get();

    if (/*temperature >= 0*/temperature != -2 && temperature != -1) {
        tempResponseDto->temperature = temperature;
        return createDtoResponse(Status::CODE_200, tempResponseDto);  
    } else if (temperature == -2) {
        return createResponse(Status::CODE_504, "Request timed out"); 
    } else {
        return createResponse(Status::CODE_500, "Failed to retrieve temperature");  
    }
}


std::future<bool> MyController::checkModuleAndUidAvailability(
    const oatpp::data::type::EnumObjectWrapper<dto::ModuleEnum, oatpp::data::type::EnumInterpreterAsString<dto::ModuleEnum, false>>& module,
    const std::string& uid) {

    auto promise = std::make_shared<std::promise<bool>>();
    auto future = promise->get_future();


    std::string normalizedUid = uid;
    if (normalizedUid.size() < 2 || normalizedUid.substr(0, 2) != "0x") { 
        normalizedUid = "0x" + normalizedUid;
    }

    m_systemModule.getAvailableModules([promise, module, normalizedUid](const std::vector<CanMessage>& responses) {
        std::unordered_set<std::string> availableModules;

        for (const auto& response : responses) {
            App_messages::Common::Probe_modules_response moduleResponse;
            auto dataCopy = response.getData();

            if (moduleResponse.Interpret_data(dataCopy)) {
                std::stringstream uidHex;
                uidHex << "0x";
                for (const auto& byte : moduleResponse.uid) {
                    uidHex << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte);
                }

                std::string uidString = uidHex.str();
                uint32_t modulePart = (response.getId() >> 4) & 0xFF;
                std::string moduleType;

                switch (static_cast<Codes::Module>(modulePart)) {
                    case Codes::Module::Core_module:
                        moduleType = "core";
                        break;
                    case Codes::Module::Control_module:
                        moduleType = "control";
                        break;
                    case Codes::Module::Sensor_module:
                        moduleType = "sensor";
                        break;
                    default:
                        moduleType = "unknown";
                        break;
                }

                std::string combinedKey = moduleType + ":" + uidString;
                availableModules.insert(combinedKey);
            }
        }

        std::string inputModuleType;
        if (module == dto::ModuleEnum::control) {
            inputModuleType = "control";
        } else if (module == dto::ModuleEnum::sensor) {
            inputModuleType = "sensor";
        } else if (module == dto::ModuleEnum::core) {
            inputModuleType = "core";
        } else {
            promise->set_value(false); 
            return;
        }

        std::string combinedKey = inputModuleType + ":" + normalizedUid;
        promise->set_value(availableModules.find(combinedKey) != availableModules.end());
    });

    return future;
}



std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::postRestart(
    const oatpp::data::type::EnumObjectWrapper<dto::ModuleEnum, oatpp::data::type::EnumInterpreterAsString<dto::ModuleEnum, false>>& module,
    const oatpp::Object<MyModuleActionRequestDto>& body) {

    if (!body || !body->uid) {
        return createResponse(Status::CODE_400, "UID is required");
    }

    std::string inputUid = body->uid->c_str();
    
    auto availabilityFuture = checkModuleAndUidAvailability(module, inputUid);
    if (availabilityFuture.wait_for(std::chrono::seconds(2)) == std::future_status::timeout) {
        return createResponse(Status::CODE_500, "Timeout while checking availability");
    }

    if (!availabilityFuture.get()) {
        return createResponse(Status::CODE_404, "Module with this UID is not available");
    }

    auto targetModuleOpt = getTargetModule(module);  
    if (!targetModuleOpt.has_value()) {
        return createResponse(Status::CODE_404, "Module not found");  
    }
    Codes::Module targetModule = targetModuleOpt.value();

    std::promise<bool> restartPromise;
    auto restartFuture = restartPromise.get_future();

    auto handlepostRestartResult = [&restartPromise](bool success) {
        restartPromise.set_value(success);
    };

    m_commonModule.sendDeviceReset(m_canRequestManager, targetModule, handlepostRestartResult);

    if (restartFuture.wait_for(std::chrono::seconds(2)) == std::future_status::timeout) {
        return createResponse(Status::CODE_500, "Timeout while restarting module");
    }

    bool success = restartFuture.get();

    if (success) {
        return createResponse(Status::CODE_200, "Successfully restarted module.");
    } else {
        return createResponse(Status::CODE_500, "Failed to restart module.");
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::postUsbBootloader(
    const oatpp::Enum<dto::ModuleEnum>::AsString& module,
    const oatpp::Object<MyModuleActionRequestDto>& body) {

    if (!body || !body->uid) {
        return createResponse(Status::CODE_400, "UID is required");
    }

    std::string inputUid = body->uid->c_str();

    auto availabilityFuture = checkModuleAndUidAvailability(module, inputUid);
    if (availabilityFuture.wait_for(std::chrono::seconds(2)) == std::future_status::timeout) {
        return createResponse(Status::CODE_500, "Timeout while checking availability");
    }

    if (!availabilityFuture.get()) {
        return createResponse(Status::CODE_404, "Module with this UID is not available");
    }

    auto targetModuleOpt = getTargetModule(module);
    if (!targetModuleOpt.has_value()) {
        return createResponse(Status::CODE_404, "Module not found");
    }
    Codes::Module targetModule = targetModuleOpt.value();

    std::promise<bool> promise;
    auto future = promise.get_future();

    auto handlepostUsbBootloader = [&promise](bool success) {
        promise.set_value(success);
    };

    m_commonModule.sendDeviceUsbBootloader(m_canRequestManager, targetModule, handlepostUsbBootloader);

    if (future.wait_for(std::chrono::seconds(2)) == std::future_status::timeout) {
        return createResponse(Status::CODE_500, "Timeout while starting USB bootloader");
    }

    bool success = future.get();

    if (success) {
        return createResponse(Status::CODE_200, "Successfully restarted module in USB bootloader mode.");
    } else {
        return createResponse(Status::CODE_500, "Failed to restart module.");
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::postCanBootloader(
    const oatpp::Enum<dto::ModuleEnum>::AsString& module,
    const oatpp::Object<MyModuleActionRequestDto>& body) {

    if (!body || !body->uid) {
        return createResponse(Status::CODE_400, "UID is required");
    }

    std::string inputUid = body->uid->c_str();

    auto availabilityFuture = checkModuleAndUidAvailability(module, inputUid);
    if (availabilityFuture.wait_for(std::chrono::seconds(2)) == std::future_status::timeout) {
        return createResponse(Status::CODE_500, "Timeout while checking availability");
    }

    if (!availabilityFuture.get()) {
        return createResponse(Status::CODE_404, "Module with this UID is not available");
    }

    auto targetModuleOpt = getTargetModule(module);
    if (!targetModuleOpt.has_value()) {
        return createResponse(Status::CODE_404, "Module not found");
    }
    Codes::Module targetModule = targetModuleOpt.value();

    std::promise<bool> promise;
    auto future = promise.get_future();

    auto handlepostCanBootloader = [&promise](bool success) {
        promise.set_value(success);
    };

    m_commonModule.sendDeviceCanBootloader(m_canRequestManager, targetModule, handlepostCanBootloader);

    if (future.wait_for(std::chrono::seconds(2)) == std::future_status::timeout) {
        return createResponse(Status::CODE_500, "Timeout while starting CAN bootloader");
    }

    bool success = future.get();

    if (success) {
        return createResponse(Status::CODE_200, "Successfully restarted module in CAN bootloader mode.");
    } else {
        return createResponse(Status::CODE_500, "Failed to restart module.");
    }
}






// ==========================================
// Control module
// ==========================================
std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::setIntensities(const oatpp::Object<MyIntensitiesDto>& body) {
    if (!body || !body->intensity || body->intensity->size() != 4) {
        return createResponse(Status::CODE_400, "Invalid intensity array. Must contain exactly 4 values.");
    }

    for (size_t i = 0; i < body->intensity->size(); i++) {
        auto intensity = body->intensity->at(i); 
        if (!intensity || *intensity < 0.0f || *intensity > 1.0f) {
            return createResponse(Status::CODE_400, "Invalid intensity value. Must be between 0.0 and 1.0.");
        }
    }

    bool success = true;
    for (size_t channel = 0; channel < body->intensity->size(); ++channel) {
        std::promise<bool> promise;
        auto future = promise.get_future();

        auto handleSetIntensityResult = [&promise](bool result) {
            promise.set_value(result);
        };

        m_controlModule.setIntensity(
            Codes::Module::Control_module,
            *body->intensity->at(channel), 
            channel,
            handleSetIntensityResult
        );

        future.wait();
        if (!future.get()) {
            success = false;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }

    if (success) {
        return createResponse(Status::CODE_200, "Intensities set successfully.");
    } else {
        return createResponse(Status::CODE_500, "Failed to set intensities.");
    }
}

std::optional<int> MyController::getTargetChannel(const dto::ChannelEnum& channel) {
    switch (channel) {
        case dto::ChannelEnum::channel0:
            return 0;
        case dto::ChannelEnum::channel1:
            return 1;
        case dto::ChannelEnum::channel2:
            return 2;
        case dto::ChannelEnum::channel3:
            return 3;
        default:
            return std::nullopt;  
    }
}


std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::setIntensity(
    const oatpp::Enum<dto::ChannelEnum>::AsString& channel, 
    const oatpp::Object<MyIntensityDto>& body
) {
    if (body->intensity < 0 || body->intensity > 1) {
        return createResponse(Status::CODE_400, "Invalid intensity. Must be between 0 and 1.");
    }

    auto targetChannelOpt = getTargetChannel(channel);
    if (!targetChannelOpt.has_value()) { 
        return createResponse(Status::CODE_400, "Invalid channel. Must be channel0, channel1, channel2, or channel3.");
    }

    int targetChannel = targetChannelOpt.value();

    std::promise<bool> promise;
    auto future = promise.get_future();

    auto handleSetIntensityResult = [&promise](bool success) {
        promise.set_value(success);
    };

    m_controlModule.setIntensity(Codes::Module::Control_module, body->intensity, targetChannel, handleSetIntensityResult);

    future.wait();
    bool success = future.get();

    if (success) {
        return createResponse(Status::CODE_200, "Intensity set successfully.");
    } else {
        return createResponse(Status::CODE_500, "Failed to set intensity.");
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getIntensity(const oatpp::Enum<dto::ChannelEnum>::AsString& channel) {
    auto intensityResponseDto = MyIntensityDto::createShared();
    std::promise<float> promise;
    auto future = promise.get_future();

    auto handleIntensityResult = [&promise](float intensity) {
        promise.set_value(intensity);
    };

    auto targetChannelOpt = getTargetChannel(channel);  
    if (!targetChannelOpt.has_value()) {
        return createResponse(Status::CODE_404, "Channel not found");
    }
    int targetChannel = targetChannelOpt.value();

   
    m_controlModule.getIntensity(m_canRequestManager, Codes::Module::Control_module, targetChannel, handleIntensityResult);
    

    future.wait();
    float intensity = future.get();

    if (intensity != -2 && intensity != -1) {
        intensityResponseDto->intensity = intensity;
        return createDtoResponse(Status::CODE_200, intensityResponseDto); 
    } else if (intensity == -2) {
        return createResponse(Status::CODE_504, "Request timed out"); 
    } else {
        return createResponse(Status::CODE_500, "Failed to retrieve intensity");  
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getLedTemperature() {
    auto tempResponseDto = MyTempDto::createShared();
    std::promise<float> promise;
    auto future = promise.get_future();

    auto handleTempResult = [&promise](float temperature) {
        promise.set_value(temperature);
    };

    m_controlModule.getLedTemperature(m_canRequestManager, Codes::Module::Control_module, handleTempResult);

    future.wait();
    float temperature = future.get();

    if (temperature >= 0) {
        tempResponseDto->temperature = temperature;
        return createDtoResponse(Status::CODE_200, tempResponseDto);  
    } else if (temperature == -2) {
        return createResponse(Status::CODE_504, "Request timed out"); 
    } else {
        return createResponse(Status::CODE_500, "Failed to retrieve LED temperature");  
    }
}




/*
std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::pingDirect() {
    oatpp::String response = "{\"message\": \"Ping direct response successful\"}";

    return createResponse(Status::CODE_200, response);
}
*/