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
        App_messages::Probe_modules_response moduleResponse;

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
                case Codes::Module::Core_device:
                    moduleInfoDto->module_type = "core";
                    break;
                case Codes::Module::Control_board:
                    moduleInfoDto->module_type = "control";
                    break;
                case Codes::Module::Sensor_board:
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

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::ping(const oatpp::Enum<dto::ModuleEnum>::AsString& module) {
    auto pingResponseDto = MyPingResponseDto::createShared();
    std::promise<float> promise;
    auto future = promise.get_future();

    auto handlePingResult = [&promise](float responseTime) {
        promise.set_value(responseTime);
    };

    Codes::Module targetModule;
    if (module == dto::ModuleEnum::control) {
        targetModule = Codes::Module::Control_board;
    } else if (module == dto::ModuleEnum::sensor) {
        targetModule = Codes::Module::Sensor_board;
    } else if (module == dto::ModuleEnum::core) {
        targetModule = Codes::Module::Core_device;
    } else {
        return createResponse(Status::CODE_404, "Module not found");
    }

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

    Codes::Module targetModule;
    if (module == dto::ModuleEnum::control) {
        targetModule = Codes::Module::Control_board;
    } else if (module == dto::ModuleEnum::sensor) {
        targetModule = Codes::Module::Sensor_board;
    } else if (module == dto::ModuleEnum::core) {
        targetModule = Codes::Module::Core_device;
    } else {
        return createResponse(Status::CODE_404, "Module not found");
    }

    m_commonModule.getCoreLoad(m_canRequestManager, targetModule, handleLoadResult);

    future.wait();
    float load = future.get();

    if (load >= 0 && load <= 1.0) {
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

    Codes::Module targetModule;
    if (module == dto::ModuleEnum::control) {
        targetModule = Codes::Module::Control_board;
    } else if (module == dto::ModuleEnum::sensor) {
        targetModule = Codes::Module::Sensor_board;
    } else if (module == dto::ModuleEnum::core) {
        targetModule = Codes::Module::Core_device;
    } else {
        return createResponse(Status::CODE_404, "Module not found");
    }

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

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::restartModule(
    const oatpp::Enum<dto::ModuleEnum>::AsString& module, 
    const oatpp::Object<MyModuleActionRequestDto>& body) {
    
    if (!body || !body->uid) {
        return createResponse(Status::CODE_400, "Invalid request. UID is required.");
    }

    std::promise<bool> promise;
    auto future = promise.get_future();

    auto handleRestartResult = [&promise](bool success) {
        promise.set_value(success);
    };

    Codes::Module targetModule;
    if (module == dto::ModuleEnum::control) {
        targetModule = Codes::Module::Control_board;
    } else if (module == dto::ModuleEnum::sensor) {
        targetModule = Codes::Module::Sensor_board;
    } else if (module == dto::ModuleEnum::core) {
        targetModule = Codes::Module::Core_device;
    } else {
        return createResponse(Status::CODE_404, "Module not found");
    }

    m_commonModule.restartModule(m_canRequestManager, targetModule, body->uid->c_str(), handleRestartResult);

    future.wait();
    bool success = future.get();

    if (success) {
        return createResponse(Status::CODE_200, "Module successfully restarted");
    } else {
        return createResponse(Status::CODE_500, "Failed to restart module");
    }
}

// ==========================================
// Control module
// ==========================================
std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::setIntensity(const oatpp::Object<MyIntensityDto>& body) {

   
    if (body->intensity < 0 || body->intensity > 1) {
        return createResponse(Status::CODE_400, "Invalid intensity. Must be between 0 and 1.");
    }

    
    if (body->channel < 0 || body->channel > 3) { 
        return createResponse(Status::CODE_400, "Invalid channel. Must be 0, 1, 2, or 3.");
    }

    std::promise<bool> promise;
    auto future = promise.get_future();

    auto handleSetIntensityResult = [&promise](bool success) {
        promise.set_value(success);
    };

    
    m_controlModule.setIntensity(Codes::Module::Control_board, body->intensity, body->channel, handleSetIntensityResult);

    future.wait();
    bool success = future.get();

    if (success) {
        return createResponse(Status::CODE_200, "Intensity set successfully.");
    } else {
        return createResponse(Status::CODE_500, "Failed to set intensity.");
    }
}



std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::pingDirect() {
    oatpp::String response = "{\"message\": \"Ping direct response successful\"}";

    return createResponse(Status::CODE_200, response);
}







