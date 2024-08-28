#include "MyController.hpp"

// ==========================================
// System Endpoints
// ==========================================
std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getSystemModules() {
    SystemModule& systemModule = SystemModule::getInstance();
    auto moduleDataFuture = systemModule.getAvailableModules();
    auto moduleData = moduleDataFuture.get();  

    if (!moduleData) {
        return createResponse(Status::CODE_500, "Failed to retrieve modules");
    }

    auto dto = oatpp::List<Object<MyModuleInfoDto>>::createShared();

    // TODO - data processing

    return createDtoResponse(Status::CODE_200, dto);
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getSystemTemperature() {
    auto dto = MyTempDto::createShared();

    SystemModule& systemModule = SystemModule::getInstance();
    float temperature = systemModule.getSystemTemperature().get();  

    if (temperature < 0.0f) {
        return createResponse(Status::CODE_500, "Failed to retrieve system temperature");
    }

    dto->temperature = temperature;
    return createDtoResponse(Status::CODE_200, dto);
}

// ==========================================
// Common Endpoints
// ==========================================
std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::ping(const oatpp::data::type::EnumObjectWrapper<dto::ModuleEnum, oatpp::data::type::EnumInterpreterAsString<dto::ModuleEnum, false> >::AsString& module) {
    float responseTime = -1.0f;

    if (module == dto::ModuleEnum::control) {
        responseTime = ControlModule::getInstance().ping().get();
    } else if (module == dto::ModuleEnum::core) {
        responseTime = CoreModule::getInstance().ping().get();
    } else if (module == dto::ModuleEnum::sensor) {
        responseTime = SensorModule::getInstance().ping().get();
    } else {
        return createResponse(Status::CODE_404, "Module not found");
    }

    if (responseTime < 0) {
        return createResponse(Status::CODE_500, "Ping failed");
    }

    auto dto = MyPingResponseDto::createShared();
    dto->time_ms = responseTime;

    return createDtoResponse(Status::CODE_200, dto);
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getLoad(const oatpp::data::type::EnumObjectWrapper<dto::ModuleEnum, oatpp::data::type::EnumInterpreterAsString<dto::ModuleEnum, false> >::AsString& module) {
    auto dto = MyLoadResponseDto::createShared();
    bool success = false;
    
    float load = 0.0f;
    int cores = 0;

    if (module == dto::ModuleEnum::control) {
        success = ControlModule::getInstance().getLoad(load, cores).get();
    } else if (module == dto::ModuleEnum::core) {
        success = CoreModule::getInstance().getLoad(load, cores).get();
    } else if (module == dto::ModuleEnum::sensor) {
        success = SensorModule::getInstance().getLoad(load, cores).get();
    } else {
        return createResponse(Status::CODE_404, "Module not found");
    }

    if (!success) {
        return createResponse(Status::CODE_500, "Failed to get load");
    }

    dto->load = load;
    dto->cores = cores;

    return createDtoResponse(Status::CODE_200, dto);
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getCoreTemp(const oatpp::data::type::EnumObjectWrapper<dto::ModuleEnum, oatpp::data::type::EnumInterpreterAsString<dto::ModuleEnum, false> >::AsString& module) {
    auto dto = MyCoreTempResponseDto::createShared();
    bool success = false;
    
    float core_temp = 0.0f;

    if (module == dto::ModuleEnum::control) {
        success = ControlModule::getInstance().getCoreTemp(core_temp).get();
    } else if (module == dto::ModuleEnum::core) {
        success = CoreModule::getInstance().getCoreTemp(core_temp).get();
    } else if (module == dto::ModuleEnum::sensor) {
        success = SensorModule::getInstance().getCoreTemp(core_temp).get();
    } else {
        return createResponse(Status::CODE_404, "Module not found");
    }

    if (!success) {
        return createResponse(Status::CODE_500, "Failed to get core temperature");
    }

    dto->core_temp = core_temp;

    return createDtoResponse(Status::CODE_200, dto);
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::restartModule(const oatpp::data::type::EnumObjectWrapper<dto::ModuleEnum, oatpp::data::type::EnumInterpreterAsString<dto::ModuleEnum, false> >::AsString& module, const oatpp::Object<MyModuleActionRequestDto>& body) {
    bool success = false;

    std::stringstream ss;
    ss << body->uid;
    std::string uid_str = ss.str();

    if (module == dto::ModuleEnum::control) {
        success = ControlModule::getInstance().restart(uid_str).get();
    } else if (module == dto::ModuleEnum::core) {
        success = CoreModule::getInstance().restart(uid_str).get();
    } else if (module == dto::ModuleEnum::sensor) {
        success = SensorModule::getInstance().restart(uid_str).get();
    } else {
        return createResponse(Status::CODE_404, "Module not found");
    }

    if (!success) {
        return createResponse(Status::CODE_500, "Failed to restart module");
    }

    return createResponse(Status::CODE_200, "Module restarted successfully");
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::bootloaderModule(const oatpp::data::type::EnumObjectWrapper<dto::ModuleEnum, oatpp::data::type::EnumInterpreterAsString<dto::ModuleEnum, false> >::AsString& module, const oatpp::Object<MyModuleActionRequestDto>& body) {
    bool success = false;

    std::stringstream ss;
    ss << body->uid;
    std::string uid_str = ss.str();

    if (module == dto::ModuleEnum::control) {
        success = ControlModule::getInstance().bootloader(uid_str).get();
    } else if (module == dto::ModuleEnum::core) {
        success = CoreModule::getInstance().bootloader(uid_str).get();
    } else if (module == dto::ModuleEnum::sensor) {
        success = SensorModule::getInstance().bootloader(uid_str).get();
    } else {
        return createResponse(Status::CODE_404, "Module not found");
    }

    if (!success) {
        return createResponse(Status::CODE_500, "Failed to boot module into bootloader mode");
    }

    return createResponse(Status::CODE_200, "Module booted into bootloader mode successfully");
}

// ==========================================
// Core module
// ==========================================
std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getSupplyType() {
    auto dto = MySupplyTypeResponseDto::createShared();

    bool adapter = false;
    bool poe = false;

    auto result = CoreModule::getInstance().getSupplyType(adapter, poe).get();  

    if (result) {
        dto->adapter = adapter;
        dto->poe = poe;
        return createDtoResponse(Status::CODE_200, dto);
    } else {
        return createResponse(Status::CODE_500, "Failed to retrieve supply type");
    }
}

// ==========================================
// Control module
// ==========================================
std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::setHeaterTemperature(const oatpp::Object<MyTempDto>& body) {
    auto result = ControlModule::getInstance().setHeaterTemperature(body->temperature).get();
    
    if (!result) {
        return createResponse(Status::CODE_500, "Failed to set heater temperature");
    }
    return createResponse(Status::CODE_200, "Heater temperature set successfully");
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getHeaterTemperature() {
    auto temperature = ControlModule::getInstance().getHeaterTemperature().get();  

    if (temperature < 0) {
        return createResponse(Status::CODE_500, "Failed to get heater temperature");
    }
    auto dto = MyTempDto::createShared();
    dto->temperature = temperature;
    return createDtoResponse(Status::CODE_200, dto);
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::disableHeater() {
    auto result = ControlModule::getInstance().disableHeater().get();

    if (!result) {
        return createResponse(Status::CODE_500, "Failed to disable heater");
    }
    return createResponse(Status::CODE_200, "Heater disabled successfully");
}

// ==========================================
// Sensor module
// ==========================================
std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getTopTemperature() {
    auto result = SensorModule::getInstance().getTopTemperature().get();  

    if (result >= 0.0f) {
        auto dto = MyTempDto::createShared();
        dto->temperature = result;
        return createDtoResponse(Status::CODE_200, dto);
    } else {
        return createResponse(Status::CODE_500, "Failed to get top sensor temperature");
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getBottomTemperature() {
    auto result = SensorModule::getInstance().getBottomTemperature().get(); 

    if (result >= 0.0f) {
        auto dto = MyTempDto::createShared();
        dto->temperature = result;
        return createDtoResponse(Status::CODE_200, dto);
    } else {
        return createResponse(Status::CODE_500, "Failed to get bottom sensor temperature");
    }
}
