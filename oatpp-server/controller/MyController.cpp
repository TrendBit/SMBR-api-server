#include "MyController.hpp"


MyController::MyController(const std::shared_ptr<oatpp::web::mime::ContentMappers>& apiContentMappers,
                           boost::asio::io_context& ioContext,
                           SystemModule& systemModule,
                           CommonModule& commonModule,
                           CanRequestManager& canRequestManager)
    : oatpp::web::server::api::ApiController(apiContentMappers)
    , m_ioContext(ioContext)
    , m_systemModule(systemModule)
    , m_commonModule(commonModule)
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
            auto moduleInfoDto = MyModuleInfoDto::createShared();
            std::stringstream uidHex;
            uidHex << "0x";
            for (const auto& byte : response.getData()) {
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

    m_commonModule.ping(m_canRequestManager, targetModule, handlePingResult);

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

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::pingWithSeq(const oatpp::Enum<dto::ModuleEnum>::AsString& module, const oatpp::Int32& seq_num) {
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

    m_commonModule.pingWithSeq(m_canRequestManager, targetModule, static_cast<uint8_t>(seq_num), handlePingResult);

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

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getLoad(const oatpp::Enum<dto::ModuleEnum>::AsString& module) {
    auto loadResponseDto = MyLoadResponseDto::createShared();
    std::promise<std::tuple<float, int>> promise;
    auto future = promise.get_future();

    auto handleLoadResult = [&promise](float load, int cores) {
        promise.set_value(std::make_tuple(load, cores));
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

    m_commonModule.getLoad(m_canRequestManager, targetModule, handleLoadResult);

    future.wait();
    auto [load, cores] = future.get();

    if (load >= 0 && cores > 0) {
        loadResponseDto->load = load;
        loadResponseDto->cores = cores;
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

    if (temperature >= 0) {
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



