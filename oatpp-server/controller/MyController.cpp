#include "MyController.hpp"

MyController::MyController(const std::shared_ptr<oatpp::web::mime::ContentMappers>& apiContentMappers,
                           boost::asio::io_context& ioContext,
                           SystemModule& systemModule,
                           CommonModule& commonModule,
                           ControlModule& controlModule,
                           CoreModule& coreModule,
                           SensorModule& sensorModule,
                           CanRequestManager& canRequestManager)
    : oatpp::web::server::api::ApiController(apiContentMappers)
    , m_ioContext(ioContext)
    , m_systemModule(systemModule)
    , m_commonModule(commonModule)
    , m_controlModule(controlModule)
    , m_coreModule(coreModule)
    , m_sensorModule(sensorModule)
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
// Core module
// ==========================================

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getShortID() {
    auto sidResponseDto = MySIDDto::createShared();
    std::promise<std::string> promise;
    auto future = promise.get_future();

    auto handleSIDResult = [&promise](std::string sid) {
        promise.set_value(sid);
    };

    m_coreModule.getShortID(m_canRequestManager, Codes::Module::Core_module, handleSIDResult);

    future.wait();
    std::string sid = future.get();

    if (sid == "timeout") {
        return createResponse(Status::CODE_504, "Request timed out");
    } else if (!sid.empty()) {
        sidResponseDto->sid = sid;
        return createDtoResponse(Status::CODE_200, sidResponseDto);
    } else {
        return createResponse(Status::CODE_500, "Failed to retrieve SID");
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getIpAddress() {
    auto ipResponseDto = MyIpDto::createShared();
    std::promise<std::string> promise;
    auto future = promise.get_future();

    auto handleIpAddressResult = [&promise](std::string ipAddress) {
        promise.set_value(ipAddress);
    };

    m_coreModule.getIpAddress(m_canRequestManager, Codes::Module::Core_module, handleIpAddressResult);

    future.wait();
    std::string ipAddress = future.get();

    if (ipAddress == "timeout") {
        return createResponse(Status::CODE_504, "Request timed out");  
    }

    if (ipAddress.empty()) {
        return createResponse(Status::CODE_500, "Failed to retrieve IP address");  
    }

    ipResponseDto->ipAddress = ipAddress;
    return createDtoResponse(Status::CODE_200, ipResponseDto); 
}


std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getHostname() {
    auto hostnameResponseDto = MyHostnameDto::createShared();
    std::promise<std::string> promise;
    auto future = promise.get_future();

    auto handleHostnameResult = [&promise](std::string hostname) {
        promise.set_value(hostname);
    };

    m_coreModule.getHostname(m_canRequestManager, Codes::Module::Core_module, handleHostnameResult);

    future.wait();
    std::string hostname = future.get();

    if (hostname == "timeout") {
        return createResponse(Status::CODE_504, "Request timed out");
    } else if (!hostname.empty()) {
        hostnameResponseDto->hostname = hostname;
        return createDtoResponse(Status::CODE_200, hostnameResponseDto);
    } else {
        return createResponse(Status::CODE_500, "Failed to retrieve hostname");
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getSerialNumber() {
    auto serialResponseDto = MySerialDto::createShared();
    std::promise<int64_t> promise;
    auto future = promise.get_future();

    auto handleSerialResult = [&promise](int64_t serial) {
        promise.set_value(serial);
    };

    m_coreModule.getSerialNumber(m_canRequestManager, Codes::Module::Core_module, handleSerialResult);

    future.wait();
    int64_t serial = future.get();

    if (serial == -2) {
        return createResponse(Status::CODE_504, "Request timed out");
    } else if (serial >= 0) {
        serialResponseDto->serial = serial;
        return createDtoResponse(Status::CODE_200, serialResponseDto);
    } else {
        return createResponse(Status::CODE_500, "Failed to retrieve serial number");
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getPowerSupplyType() {
    auto supplyTypeResponseDto = MySupplyTypeDto::createShared();
    std::promise<std::tuple<bool, bool, bool, bool>> promise;
    auto future = promise.get_future();

    auto handleSupplyTypeResult = [&promise](bool success, bool vin, bool poe, bool poe_hb) {
        promise.set_value(std::make_tuple(success, vin, poe, poe_hb));
    };

    m_coreModule.getPowerSupplyType(m_canRequestManager, Codes::Module::Core_module, handleSupplyTypeResult);

    future.wait();
    auto [success, vin, poe, poe_hb] = future.get();

    if (!success) {
        return createResponse(Status::CODE_504, "Request timed out");
    }

    supplyTypeResponseDto->vin = vin;
    supplyTypeResponseDto->poe = poe;
    supplyTypeResponseDto->poe_hb = poe_hb;
    return createDtoResponse(Status::CODE_200, supplyTypeResponseDto);
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getVoltage5V() {
    auto voltageResponseDto = MyVoltageDto::createShared();
    std::promise<float> promise;
    auto future = promise.get_future();

    auto handleVoltageResult = [&promise](float voltage) {
        promise.set_value(voltage);
    };

    m_coreModule.getVoltage5V(m_canRequestManager, Codes::Module::Core_module, handleVoltageResult);

    future.wait();
    float voltage = future.get();

    if (voltage == -2.0f) {
        return createResponse(Status::CODE_504, "Request timed out");
    } else if (voltage >= 0.0f) {
        voltageResponseDto->voltage = voltage;
        return createDtoResponse(Status::CODE_200, voltageResponseDto);
    } else {
        return createResponse(Status::CODE_500, "Failed to retrieve voltage");
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getVoltageVIN() {
    auto voltageResponseDto = MyVoltageDto::createShared();
    std::promise<float> promise;
    auto future = promise.get_future();

    auto handleVoltageResult = [&promise](float voltage) {
        promise.set_value(voltage);
    };

    m_coreModule.getVoltageVIN(m_canRequestManager, Codes::Module::Core_module, handleVoltageResult);

    future.wait();
    float voltage = future.get();

    if (voltage == -2.0f) {
        return createResponse(Status::CODE_504, "Request timed out");
    } else if (voltage > -1.0f) {
        voltageResponseDto->voltage = voltage;
        return createDtoResponse(Status::CODE_200, voltageResponseDto);
    } else {
        return createResponse(Status::CODE_500, "Failed to retrieve VIN voltage");
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getPoEVoltage() {
    auto voltageResponseDto = MyVoltageDto::createShared();
    std::promise<float> promise;
    auto future = promise.get_future();

    auto handlePoEResult = [&promise](float voltage) {
        promise.set_value(voltage);
    };

    m_coreModule.getVoltagePoE(m_canRequestManager, Codes::Module::Core_module, handlePoEResult);

    future.wait();
    float voltage = future.get();

    if (voltage == -2.0f) {
        return createResponse(Status::CODE_504, "Request timed out");
    } else if (voltage > -1.0f) {
        voltageResponseDto->voltage = voltage;
        return createDtoResponse(Status::CODE_200, voltageResponseDto);
    } else {
        return createResponse(Status::CODE_500, "Failed to retrieve POE voltage");
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getCurrentConsumption() {
    auto currentResponseDto = MyCurrentDto::createShared();
    std::promise<float> promise;
    auto future = promise.get_future();

    auto handleCurrentResult = [&promise](float current) {
        promise.set_value(current);
    };

    m_coreModule.getCurrentConsumption(m_canRequestManager, Codes::Module::Core_module, handleCurrentResult);

    future.wait();
    float current = future.get();

    if (current == -2.0f) {
        return createResponse(Status::CODE_504, "Request timed out");
    } else if (current > -1.0f && current <= 10.0f) {
        currentResponseDto->current = current;
        return createDtoResponse(Status::CODE_200, currentResponseDto);
    } else {
        return createResponse(Status::CODE_500, "Failed to retrieve current consumption");
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getPowerDraw() {
    auto powerDrawResponseDto = MyPowerDrawDto::createShared();
    std::promise<float> promise;
    auto future = promise.get_future();

    auto handlePowerDrawResult = [&promise](float powerDraw) {
        promise.set_value(powerDraw);
    };

    m_coreModule.getPowerDraw(m_canRequestManager, Codes::Module::Core_module, handlePowerDrawResult);

    future.wait();
    float powerDraw = future.get();

    if (powerDraw == -2.0f) {
        return createResponse(Status::CODE_504, "Request timed out");
    } else if (powerDraw >= 0.0f && powerDraw <= 100.0f) {
        powerDrawResponseDto->power_draw = powerDraw;
        return createDtoResponse(Status::CODE_200, powerDrawResponseDto);
    } else {
        return createResponse(Status::CODE_500, "Failed to retrieve power draw");
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

    if (temperature == -30) {
        return createResponse(Status::CODE_504, "Request timed out");
    } else if (temperature == -100) {
        return createResponse(Status::CODE_503, "Module not available"); 
    } else if (temperature >= -30) {
        tempResponseDto->temperature = temperature;
        return createDtoResponse(Status::CODE_200, tempResponseDto); 
    } else {
        return createResponse(Status::CODE_500, "Failed to retrieve LED temperature"); 
    }
}


std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::setHeaterIntensity(const oatpp::Object<MyIntensityDto>& body) {
    if (!body || body->intensity < -1.0f || body->intensity > 1.0f) {
        return createResponse(Status::CODE_400, "Invalid intensity value. Must be between -1.0 and 1.0.");
    }

    std::promise<bool> promise;
    auto future = promise.get_future();

    auto handleSetIntensityResult = [&promise](bool success) {
        promise.set_value(success);
    };

    m_controlModule.setHeaterIntensity(Codes::Module::Control_module, body->intensity, handleSetIntensityResult);

    future.wait();
    bool success = future.get();

    if (success) {
        return createResponse(Status::CODE_200, "Intensity set successfully.");
    } else {
        return createResponse(Status::CODE_500, "Failed to set heater intensity.");
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getHeaterIntensity() {
    auto intensityResponseDto = MyIntensityDto::createShared();
    std::promise<float> promise;
    auto future = promise.get_future();

    auto handleIntensityResult = [&promise](float intensity) {
        promise.set_value(intensity);
    };

    m_controlModule.getHeaterIntensity(m_canRequestManager, Codes::Module::Control_module, handleIntensityResult);

    future.wait();
    float intensity = future.get();

    if (intensity >= -1.0f && intensity <= 1.0f) {
        intensityResponseDto->intensity = intensity;
        return createDtoResponse(Status::CODE_200, intensityResponseDto);  
    } else if (intensity == -2) {
        return createResponse(Status::CODE_504, "Request timed out"); 
    } else {
        return createResponse(Status::CODE_500, "Failed to retrieve heater intensity");  
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::setHeaterTargetTemperature(const oatpp::Object<MyTempDto>& body) {
    if (!body || body->temperature < 0.0f) {
        return createResponse(Status::CODE_400, "Invalid target temperature. Must be a positive value.");
    }

    std::promise<bool> promise;
    auto future = promise.get_future();

    auto handleSetTargetTempResult = [&promise](bool success) {
        promise.set_value(success);
    };

    m_controlModule.setHeaterTargetTemperature(Codes::Module::Control_module, body->temperature, handleSetTargetTempResult);

    future.wait();
    bool success = future.get();

    if (success) {
        return createResponse(Status::CODE_200, "Target temperature set successfully.");
    } else {
        return createResponse(Status::CODE_500, "Failed to set target temperature.");
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getHeaterTargetTemperature() {
    auto tempResponseDto = MyTempDto::createShared();
    std::promise<float> promise;
    auto future = promise.get_future();

    auto handleTempResult = [&promise](float temperature) {
        promise.set_value(temperature);
    };

    m_controlModule.getHeaterTargetTemperature(m_canRequestManager, Codes::Module::Control_module, handleTempResult);

    future.wait();
    float temperature = future.get();

    if (temperature == -30) {
        return createResponse(Status::CODE_504, "Request timed out");
    } else if (temperature == -100) {
        return createResponse(Status::CODE_503, "Module not available");
    } else if (temperature >= -30) {
        tempResponseDto->temperature = temperature;
        return createDtoResponse(Status::CODE_200, tempResponseDto);
    } else {
        return createResponse(Status::CODE_500, "Failed to retrieve heater target temperature");
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getHeaterPlateTemperature() {
    auto plateTempResponseDto = MyTempDto::createShared();
    std::promise<float> promise;
    auto future = promise.get_future();

    auto handlePlateTempResult = [&promise](float plateTemperature) {
        promise.set_value(plateTemperature);
    };

    m_controlModule.getHeaterPlateTemperature(m_canRequestManager, Codes::Module::Control_module, handlePlateTempResult);

    future.wait();
    float plateTemperature = future.get();

    if (plateTemperature == -30) {
        return createResponse(Status::CODE_504, "Request timed out");
    } else if (plateTemperature == -100) {
        return createResponse(Status::CODE_503, "Module not available");
    } else if (plateTemperature >= -30) {
        plateTempResponseDto->temperature = plateTemperature;
        return createDtoResponse(Status::CODE_200, plateTempResponseDto);
    } else {
        return createResponse(Status::CODE_500, "Failed to retrieve heater plate temperature");
    }
}


std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::turnOffHeater() {
    std::promise<bool> promise;
    auto future = promise.get_future();

    auto handleTurnOffResult = [&promise](bool success) {
        promise.set_value(success);
    };

    m_controlModule.turnOffHeater(Codes::Module::Control_module, handleTurnOffResult);

    future.wait();
    bool success = future.get();

    if (success) {
        return createResponse(Status::CODE_200, "Heater was turned off.");
    } else {
        return createResponse(Status::CODE_500, "Failed to turn off heater.");
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::setCuvettePumpSpeed(const oatpp::Object<MySpeedDto>& body) {
    if (!body || body->speed < -1.0f || body->speed > 1.0f) {
        return createResponse(Status::CODE_400, "Invalid speed value. Must be between -1.0 and 1.0.");
    }

    std::promise<bool> promise;
    auto future = promise.get_future();

    auto handleSetSpeedResult = [&promise](bool success) {
        promise.set_value(success);
    };

    m_controlModule.setCuvettePumpSpeed(Codes::Module::Control_module, body->speed, handleSetSpeedResult);

    future.wait();
    bool success = future.get();

    if (success) {
        return createResponse(Status::CODE_200, "Speed set successfully.");
    } else {
        return createResponse(Status::CODE_500, "Failed to set pump speed.");
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getCuvettePumpSpeed() {
    auto speedResponseDto = MySpeedDto::createShared();
    std::promise<float> promise;
    auto future = promise.get_future();

    auto handleSpeedResult = [&promise](float speed) {
        promise.set_value(speed);
    };

    m_controlModule.getCuvettePumpSpeed(m_canRequestManager, Codes::Module::Control_module, handleSpeedResult);

    future.wait();
    float speed = future.get();

    if (speed >= -1.0f && speed <= 1.0f) {
        speedResponseDto->speed = speed;
        return createDtoResponse(Status::CODE_200, speedResponseDto);  
    } else if (speed == -2) {
        return createResponse(Status::CODE_504, "Request timed out"); 
    } else {
        return createResponse(Status::CODE_500, "Failed to retrieve pump speed");  
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::setCuvettePumpFlowrate(const oatpp::Object<MyFlowrateDto>& body) {
    if (!body || body->flowrate < -1000.0f || body->flowrate > 1000.0f) {
        return createResponse(Status::CODE_400, "Invalid flowrate value. Must be between -1000.0 and 1000.0.");
    }

    std::promise<bool> promise;
    auto future = promise.get_future();

    auto handleSetFlowrateResult = [&promise](bool success) {
        promise.set_value(success);
    };

    m_controlModule.setCuvettePumpFlowrate(Codes::Module::Control_module, body->flowrate, handleSetFlowrateResult);

    future.wait();
    bool success = future.get();

    if (success) {
        return createResponse(Status::CODE_200, "Flowrate set successfully.");
    } else {
        return createResponse(Status::CODE_500, "Failed to set cuvette pump flowrate.");
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getCuvettePumpFlowrate() {
    auto flowrateResponseDto = MyFlowrateDto::createShared();
    std::promise<float> promise;
    auto future = promise.get_future();

    auto handleFlowrateResult = [&promise](float flowrate) {
        promise.set_value(flowrate);
    };

    m_controlModule.getCuvettePumpFlowrate(m_canRequestManager, Codes::Module::Control_module, handleFlowrateResult);

    future.wait();
    float flowrate = future.get();

    if (flowrate == -2000) {
        return createResponse(Status::CODE_504, "Request timed out"); 
    } else if (flowrate >= -1000.0f && flowrate <= 1000.0f) {
        flowrateResponseDto->flowrate = flowrate;
        return createDtoResponse(Status::CODE_200, flowrateResponseDto);  
    } else {
        return createResponse(Status::CODE_500, "Failed to retrieve cuvette pump flowrate");  
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::moveCuvettePump(const oatpp::Object<MyMoveDto>& body) {
    auto moveResponseDto = MyMoveDto::createShared();
    std::promise<bool> promise;
    auto future = promise.get_future();

    auto handleMoveResult = [&promise](bool success) {
        promise.set_value(success);
    };

    m_controlModule.moveCuvettePump(Codes::Module::Control_module, body->volume, body->flowrate, handleMoveResult);

    future.wait();
    bool success = future.get();

    if (success) {
        moveResponseDto->volume = body->volume;
        moveResponseDto->flowrate = body->flowrate;
        return createDtoResponse(Status::CODE_200, moveResponseDto);  
    } else {
        return createResponse(Status::CODE_500, "Failed to start moving liquid");  
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::primeCuvettePump() {
    std::promise<bool> promise;
    auto future = promise.get_future();

    auto handlePrimeResult = [&promise](bool success) {
        promise.set_value(success);
    };

    m_controlModule.primeCuvettePump(Codes::Module::Control_module, handlePrimeResult);

    future.wait();
    bool success = future.get();

    if (success) {
        return createResponse(Status::CODE_200, "Cuvette pump priming was started.");
    } else {
        return createResponse(Status::CODE_500, "Failed to start cuvette pump priming.");
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::purgeCuvettePump() {
    std::promise<bool> promise;
    auto future = promise.get_future();

    auto handlePurgeResult = [&promise](bool success) {
        promise.set_value(success);
    };

    m_controlModule.purgeCuvettePump(Codes::Module::Control_module, handlePurgeResult);

    future.wait();
    bool success = future.get();

    if (success) {
        return createResponse(Status::CODE_200, "Cuvette pump purging was started.");
    } else {
        return createResponse(Status::CODE_500, "Failed to start cuvette pump purging.");
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::stopCuvettePump() {
    std::promise<bool> promise;
    auto future = promise.get_future();

    auto handleStopResult = [&promise](bool success) {
        promise.set_value(success);
    };

    m_controlModule.stopCuvettePump(Codes::Module::Control_module, handleStopResult);

    future.wait();
    bool success = future.get();

    if (success) {
        return createResponse(Status::CODE_200, "Cuvette pump was stopped.");
    } else {
        return createResponse(Status::CODE_500, "Failed to stop cuvette pump.");
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::setAeratorSpeed(const oatpp::Object<MySpeedDto>& body) {
    if (!body || body->speed < 0.0f || body->speed > 1.0f) {
        return createResponse(Status::CODE_400, "Invalid speed value. Must be between 0.0 and 1.0.");
    }

    std::promise<bool> promise;
    auto future = promise.get_future();

    auto handleSetSpeedResult = [&promise](bool success) {
        promise.set_value(success);
    };

    m_controlModule.setAeratorSpeed(Codes::Module::Control_module, body->speed, handleSetSpeedResult);

    future.wait();
    bool success = future.get();

    if (success) {
        return createResponse(Status::CODE_200, "Speed set successfully.");
    } else {
        return createResponse(Status::CODE_500, "Failed to set aerator speed.");
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getAeratorSpeed() {
    auto speedResponseDto = MySpeedDto::createShared();
    std::promise<float> promise;
    auto future = promise.get_future();

    auto handleSpeedResult = [&promise](float speed) {
        promise.set_value(speed);
    };

    m_controlModule.getAeratorSpeed(m_canRequestManager, Codes::Module::Control_module, handleSpeedResult);

    future.wait();
    float speed = future.get();

    if (speed >= 0.0f && speed <= 1.0f) {
        speedResponseDto->speed = speed;
        return createDtoResponse(Status::CODE_200, speedResponseDto);  
    } else if (speed == -2) {
        return createResponse(Status::CODE_504, "Request timed out"); 
    } else {
        return createResponse(Status::CODE_500, "Failed to retrieve aerator speed");  
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::setAeratorFlowrate(const oatpp::Object<MyFlowrateDto>& body) {
    if (!body || body->flowrate < 0.0f || body->flowrate > 5000.0f) {
        return createResponse(Status::CODE_400, "Invalid flowrate value. Must be between 10.0 and 5000.0 ml/min.");
    }

    std::promise<bool> promise;
    auto future = promise.get_future();

    auto handleSetFlowrateResult = [&promise](bool success) {
        promise.set_value(success);
    };

    m_controlModule.setAeratorFlowrate(Codes::Module::Control_module, body->flowrate, handleSetFlowrateResult);

    future.wait();
    bool success = future.get();

    if (success) {
        return createResponse(Status::CODE_200, "Flowrate set successfully.");
    } else {
        return createResponse(Status::CODE_500, "Failed to set aerator flowrate.");
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getAeratorFlowrate() {
    auto flowrateResponseDto = MyFlowrateDto::createShared();
    std::promise<float> promise;
    auto future = promise.get_future();

    auto handleFlowrateResult = [&promise](float flowrate) {
        promise.set_value(flowrate);
    };

    m_controlModule.getAeratorFlowrate(m_canRequestManager, Codes::Module::Control_module, handleFlowrateResult);

    future.wait();
    float flowrate = future.get();

    if (flowrate >= 0.0f && flowrate <= 5000.0f) {
        flowrateResponseDto->flowrate = flowrate;
        return createDtoResponse(Status::CODE_200, flowrateResponseDto);  
    } else if (flowrate == -2) {
        return createResponse(Status::CODE_504, "Request timed out"); 
    } else {
        return createResponse(Status::CODE_500, "Failed to retrieve aerator flowrate");  
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::moveAerator(const oatpp::Object<MyMoveDto>& body) {
    auto moveResponseDto = MyMoveDto::createShared();
    std::promise<bool> promise;
    auto future = promise.get_future();

    auto handleMoveResult = [&promise](bool success) {
        promise.set_value(success);
    };

    m_controlModule.moveAerator(Codes::Module::Control_module, body->volume, body->flowrate, handleMoveResult);

    future.wait();
    bool success = future.get();

    if (success) {
        moveResponseDto->volume = body->volume;
        moveResponseDto->flowrate = body->flowrate;
        return createDtoResponse(Status::CODE_200, moveResponseDto);  
    } else {
        return createResponse(Status::CODE_500, "Failed to start moving air");
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::stopAerator() {
    std::promise<bool> promise;
    auto future = promise.get_future();

    auto handleStopResult = [&promise](bool success) {
        promise.set_value(success);
    };

    m_controlModule.stopAerator(Codes::Module::Control_module, handleStopResult);

    future.wait();
    bool success = future.get();

    if (success) {
        return createResponse(Status::CODE_200, "Aerator was stopped.");
    } else {
        return createResponse(Status::CODE_500, "Failed to stop aerator.");
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::setMixerSpeed(const oatpp::Object<MySpeedDto>& body) {
    if (!body || body->speed < 0.0f || body->speed > 1.0f) {
        return createResponse(Status::CODE_400, "Invalid speed value. Must be between 0.0 and 1.0.");
    }

    std::promise<bool> promise;
    auto future = promise.get_future();

    auto handleSetSpeedResult = [&promise](bool success) {
        promise.set_value(success);
    };

    m_controlModule.setMixerSpeed(Codes::Module::Control_module, body->speed, handleSetSpeedResult);

    future.wait();
    bool success = future.get();

    if (success) {
        return createResponse(Status::CODE_200, "Mixer speed set successfully.");
    } else {
        return createResponse(Status::CODE_500, "Failed to set mixer speed.");
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getMixerSpeed() {
    auto speedResponseDto = MySpeedDto::createShared();
    std::promise<float> promise;
    auto future = promise.get_future();

    auto handleSpeedResult = [&promise](float speed) {
        promise.set_value(speed);
    };

    m_controlModule.getMixerSpeed(m_canRequestManager, Codes::Module::Control_module, handleSpeedResult);

    future.wait();
    float speed = future.get();

    if (speed >= 0.0f && speed <= 1.0f) {
        speedResponseDto->speed = speed;
        return createDtoResponse(Status::CODE_200, speedResponseDto);
    } else if (speed == -2) {
        return createResponse(Status::CODE_504, "Request timed out");
    } else {
        return createResponse(Status::CODE_500, "Failed to retrieve mixer speed");
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::setMixerRpm(const oatpp::Object<MyRpmDto>& body) {
    if (!body || body->rpm < 0.0f || body->rpm > 10000.0f) {
        return createResponse(Status::CODE_400, "Invalid RPM value. Must be between 0 and 10000.");
    }

    std::promise<bool> promise;
    auto future = promise.get_future();

    auto handleSetRpmResult = [&promise](bool success) {
        promise.set_value(success);
    };

    m_controlModule.setMixerRpm(Codes::Module::Control_module, body->rpm, handleSetRpmResult);

    future.wait();
    bool success = future.get();

    if (success) {
        return createResponse(Status::CODE_200, "Target RPM set successfully.");
    } else {
        return createResponse(Status::CODE_500, "Failed to set mixer RPM.");
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getMixerRpm() {
    auto rpmResponseDto = MyRpmDto::createShared();
    std::promise<int> promise;
    auto future = promise.get_future();

    auto handleRpmResult = [&promise](int rpm) {
        promise.set_value(rpm);
    };

    m_controlModule.getMixerRpm(m_canRequestManager, Codes::Module::Control_module, handleRpmResult);

    future.wait();
    int rpm = future.get();

    if (rpm >= 0 && rpm <= 10000) {
        rpmResponseDto->rpm = static_cast<float>(rpm);
        return createDtoResponse(Status::CODE_200, rpmResponseDto);  
    } else if (rpm == -2) {
        return createResponse(Status::CODE_504, "Request timed out"); 
    } else {
        return createResponse(Status::CODE_500, "Failed to retrieve mixer RPM");  
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::stirMixer(const oatpp::Object<MyStirDto>& body) {
    if (!body || body->rpm < 0.0f || body->rpm > 10000.0f || body->time < 0.0f || body->time > 3600.0f) {
        return createResponse(Status::CODE_400, "Invalid RPM or time value. RPM must be between 0 and 10000, and time must be between 0 and 3600 seconds.");
    }

    std::promise<bool> promise;
    auto future = promise.get_future();

    auto handleStirResult = [&promise](bool success) {
        promise.set_value(success);
    };

    m_controlModule.stirMixer(Codes::Module::Control_module, body->rpm, body->time, handleStirResult);

    future.wait();
    bool success = future.get();

    if (success) {
        return createResponse(Status::CODE_200, "Stirring started successfully.");
    } else {
        return createResponse(Status::CODE_500, "Failed to start stirring.");
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::stopMixer() {
    std::promise<bool> promise;
    auto future = promise.get_future();

    auto handleStopResult = [&promise](bool success) {
        promise.set_value(success);
    };

    m_controlModule.stopMixer(Codes::Module::Control_module, handleStopResult);

    future.wait();
    bool success = future.get();

    if (success) {
        return createResponse(Status::CODE_200, "Mixer was stopped.");
    } else {
        return createResponse(Status::CODE_500, "Failed to stop the mixer.");
    }
}

// ==========================================
// Sensor module
// ==========================================

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getBottleTemperature() {
    auto tempResponseDto = MyTempDto::createShared();
    std::promise<float> promise;
    auto future = promise.get_future();

    auto handleTemperatureResult = [&promise](float temperature) {
        promise.set_value(temperature);
    };

    m_sensorModule.getBottleTemperature(m_canRequestManager, Codes::Module::Sensor_module, handleTemperatureResult);

    future.wait();
    float temperature = future.get();

    if (temperature > -30) {
        tempResponseDto->temperature = temperature;
        return createDtoResponse(Status::CODE_200, tempResponseDto);
    } else if (temperature == -100) {
        return createResponse(Status::CODE_404, "Bottle temperature not available");
    } else if (temperature == -30) {
        return createResponse(Status::CODE_504, "Request timed out");
    } else {
        return createResponse(Status::CODE_500, "Failed to retrieve temperature");
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getTopMeasuredTemperature() {
    auto tempResponseDto = MyTempDto::createShared();
    std::promise<float> promise;
    auto future = promise.get_future();

    auto handleTemperatureResult = [&promise](float temperature) {
        promise.set_value(temperature);
    };

    m_sensorModule.getTopMeasuredTemperature(m_canRequestManager, Codes::Module::Sensor_module, handleTemperatureResult);

    future.wait();
    float temperature = future.get();

    if (temperature > -30) {
        tempResponseDto->temperature = temperature;
        return createDtoResponse(Status::CODE_200, tempResponseDto);
    } else if (temperature == -100) {
        return createResponse(Status::CODE_404, "Top temperature not available");
    } else if (temperature == -30) {
        return createResponse(Status::CODE_504, "Request timed out");
    } else {
        return createResponse(Status::CODE_500, "Failed to retrieve temperature");
    }
}




/*
std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::pingDirect() {
    oatpp::String response = "{\"message\": \"Ping direct response successful\"}";

    return createResponse(Status::CODE_200, response);
}
*/