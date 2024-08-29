#include "MyController.hpp"
#include <iostream>


std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getSystemModules() {
    auto promise = std::make_shared<std::promise<std::optional<std::vector<uint8_t>>>>();
    SystemModule::getInstance().getAvailableModules(m_ioContext, [promise](std::optional<std::vector<uint8_t>> moduleData) {
        promise->set_value(moduleData);
    });

    auto future = promise->get_future();
    auto moduleData = future.get();

    if (!moduleData) {
        return createResponse(Status::CODE_500, "Failed to retrieve modules");
    }

    auto dto = oatpp::List<Object<MyModuleInfoDto>>::createShared();
    // TODO: Process moduleData into dto

    return createDtoResponse(Status::CODE_200, dto);
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getSystemTemperature() {
    auto promise = std::make_shared<std::promise<float>>();
    SystemModule::getInstance().getSystemTemperature(m_ioContext, [promise](float temperature) {
        promise->set_value(temperature);
    });

    auto future = promise->get_future();
    auto temperature = future.get();

    if (temperature < 0.0f) {
        return createResponse(Status::CODE_500, "Failed to retrieve system temperature");
    }

    auto dto = MyTempDto::createShared();
    dto->temperature = temperature;
    return createDtoResponse(Status::CODE_200, dto);
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::ping(const oatpp::data::type::EnumObjectWrapper<dto::ModuleEnum, oatpp::data::type::EnumInterpreterAsString<dto::ModuleEnum, false>>::AsString& module) {
    std::cout << "MyController::ping - Start" << std::endl;
    auto promise = std::make_shared<std::promise<float>>();

    if (module == dto::ModuleEnum::control) {
        std::cout << "MyController::ping - Pinging ControlModule" << std::endl;
        ControlModule::getInstance().ping(m_ioContext, [promise](float responseTime) {
            std::cout << "MyController::ping - ControlModule response time: " << responseTime << " ms" << std::endl;
            promise->set_value(responseTime);
        });
    } else if (module == dto::ModuleEnum::core) {
        std::cout << "MyController::ping - Pinging CoreModule" << std::endl;
        CoreModule::getInstance().ping(m_ioContext, [promise](float responseTime) {
            std::cout << "MyController::ping - CoreModule response time: " << responseTime << " ms" << std::endl;
            promise->set_value(responseTime);
        });
    } else if (module == dto::ModuleEnum::sensor) {
        std::cout << "MyController::ping - Pinging SensorModule" << std::endl;
        SensorModule::getInstance().ping(m_ioContext, [promise](float responseTime) {
            std::cout << "MyController::ping - SensorModule response time: " << responseTime << " ms" << std::endl;
            promise->set_value(responseTime);
        });
    } else {
        std::cout << "MyController::ping - Module not found" << std::endl;
        return createResponse(Status::CODE_404, "Module not found");
    }

    auto future = promise->get_future();
    auto responseTime = future.get();

    if (responseTime < 0) {
        std::cout << "MyController::ping - Ping failed" << std::endl;
        return createResponse(Status::CODE_500, "Ping failed");
    }

    std::cout << "MyController::ping - Ping successful, time: " << responseTime << " ms" << std::endl;
    auto dto = MyPingResponseDto::createShared();
    dto->time_ms = responseTime;
    return createDtoResponse(Status::CODE_200, dto);
}


std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getLoad(const oatpp::data::type::EnumObjectWrapper<dto::ModuleEnum, oatpp::data::type::EnumInterpreterAsString<dto::ModuleEnum, false>>::AsString& module) {
    auto promise = std::make_shared<std::promise<std::tuple<bool, float, int>>>();

    if (module == dto::ModuleEnum::control) {
        ControlModule::getInstance().getLoad(m_ioContext, [promise](bool success, float load, int cores) {
            promise->set_value(std::make_tuple(success, load, cores));
        });
    } else if (module == dto::ModuleEnum::core) {
        CoreModule::getInstance().getLoad(m_ioContext, [promise](bool success, float load, int cores) {
            promise->set_value(std::make_tuple(success, load, cores));
        });
    } else if (module == dto::ModuleEnum::sensor) {
        SensorModule::getInstance().getLoad(m_ioContext, [promise](bool success, float load, int cores) {
            promise->set_value(std::make_tuple(success, load, cores));
        });
    } else {
        return createResponse(Status::CODE_404, "Module not found");
    }

    auto future = promise->get_future();
    auto [success, load, cores] = future.get();

    if (!success) {
        return createResponse(Status::CODE_500, "Failed to get load");
    }

    auto dto = MyLoadResponseDto::createShared();
    dto->load = load;
    dto->cores = cores;
    return createDtoResponse(Status::CODE_200, dto);
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getCoreTemp(const oatpp::data::type::EnumObjectWrapper<dto::ModuleEnum, oatpp::data::type::EnumInterpreterAsString<dto::ModuleEnum, false>>::AsString& module) {
    auto promise = std::make_shared<std::promise<std::tuple<bool, float>>>();

    if (module == dto::ModuleEnum::control) {
        ControlModule::getInstance().getCoreTemp(m_ioContext, [promise](bool success, float core_temp) {
            promise->set_value(std::make_tuple(success, core_temp));
        });
    } else if (module == dto::ModuleEnum::core) {
        CoreModule::getInstance().getCoreTemp(m_ioContext, [promise](bool success, float core_temp) {
            promise->set_value(std::make_tuple(success, core_temp));
        });
    } else if (module == dto::ModuleEnum::sensor) {
        SensorModule::getInstance().getCoreTemp(m_ioContext, [promise](bool success, float core_temp) {
            promise->set_value(std::make_tuple(success, core_temp));
        });
    } else {
        return createResponse(Status::CODE_404, "Module not found");
    }

    auto future = promise->get_future();
    auto [success, core_temp] = future.get();

    if (!success) {
        return createResponse(Status::CODE_500, "Failed to get core temperature");
    }

    auto dto = MyCoreTempResponseDto::createShared();
    dto->core_temp = core_temp;
    return createDtoResponse(Status::CODE_200, dto);
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::restartModule(const oatpp::data::type::EnumObjectWrapper<dto::ModuleEnum, oatpp::data::type::EnumInterpreterAsString<dto::ModuleEnum, false>>::AsString& module,
    const oatpp::Object<MyModuleActionRequestDto>& body) {

    auto promise = std::make_shared<std::promise<bool>>();
    auto uid = std::to_string(body->uid);

    if (module == dto::ModuleEnum::control) {
        ControlModule::getInstance().restart(m_ioContext, uid, [promise](bool success) {
            promise->set_value(success);
        });
    } else if (module == dto::ModuleEnum::core) {
        CoreModule::getInstance().restart(m_ioContext, uid, [promise](bool success) {
            promise->set_value(success);
        });
    } else if (module == dto::ModuleEnum::sensor) {
        SensorModule::getInstance().restart(m_ioContext, uid, [promise](bool success) {
            promise->set_value(success);
        });
    } else {
        return createResponse(Status::CODE_404, "Module not found");
    }

    auto future = promise->get_future();
    auto success = future.get();

    if (!success) {
        return createResponse(Status::CODE_500, "Failed to restart module");
    }

    return createResponse(Status::CODE_200, "Module restarted successfully");
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::bootloaderModule(const oatpp::data::type::EnumObjectWrapper<dto::ModuleEnum, oatpp::data::type::EnumInterpreterAsString<dto::ModuleEnum, false>>::AsString& module,
    const oatpp::Object<MyModuleActionRequestDto>& body) {

    auto promise = std::make_shared<std::promise<bool>>();
    auto uid = std::to_string(body->uid);

    if (module == dto::ModuleEnum::control) {
        ControlModule::getInstance().bootloader(m_ioContext, uid, [promise](bool success) {
            promise->set_value(success);
        });
    } else if (module == dto::ModuleEnum::core) {
        CoreModule::getInstance().bootloader(m_ioContext, uid, [promise](bool success) {
            promise->set_value(success);
        });
    } else if (module == dto::ModuleEnum::sensor) {
        SensorModule::getInstance().bootloader(m_ioContext, uid, [promise](bool success) {
            promise->set_value(success);
        });
    } else {
        return createResponse(Status::CODE_404, "Module not found");
    }

    auto future = promise->get_future();
    auto success = future.get();

    if (!success) {
        return createResponse(Status::CODE_500, "Failed to boot module into bootloader mode");
    }

    return createResponse(Status::CODE_200, "Module booted into bootloader mode successfully");
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getSupplyType() {
    auto promise = std::make_shared<std::promise<std::tuple<bool, bool, bool>>>();

    CoreModule::getInstance().getSupplyType(m_ioContext, [promise](bool success, bool adapter, bool poe) {
        promise->set_value(std::make_tuple(success, adapter, poe));
    });

    auto future = promise->get_future();
    auto [success, adapter, poe] = future.get();

    if (!success) {
        return createResponse(Status::CODE_500, "Failed to retrieve supply type");
    }

    auto dto = MySupplyTypeResponseDto::createShared();
    dto->adapter = adapter;
    dto->poe = poe;
    return createDtoResponse(Status::CODE_200, dto);
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::setHeaterTemperature(const oatpp::Object<MyTempDto>& body) {
    auto promise = std::make_shared<std::promise<bool>>();

    ControlModule::getInstance().setHeaterTemperature(m_ioContext, body->temperature, [promise](bool success) {
        promise->set_value(success);
    });

    auto future = promise->get_future();
    auto success = future.get();

    if (!success) {
        return createResponse(Status::CODE_500, "Failed to set heater temperature");
    }

    return createResponse(Status::CODE_200, "Heater temperature set successfully");
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getHeaterTemperature() {
    auto promise = std::make_shared<std::promise<float>>();

    ControlModule::getInstance().getHeaterTemperature(m_ioContext, [promise](float temperature) {
        promise->set_value(temperature);
    });

    auto future = promise->get_future();
    auto temperature = future.get();

    if (temperature < 0) {
        return createResponse(Status::CODE_500, "Failed to get heater temperature");
    }

    auto dto = MyTempDto::createShared();
    dto->temperature = temperature;
    return createDtoResponse(Status::CODE_200, dto);
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::disableHeater() {
    auto promise = std::make_shared<std::promise<bool>>();

    ControlModule::getInstance().disableHeater(m_ioContext, [promise](bool success) {
        promise->set_value(success);
    });

    auto future = promise->get_future();
    auto success = future.get();

    if (!success) {
        return createResponse(Status::CODE_500, "Failed to disable heater");
    }

    return createResponse(Status::CODE_200, "Heater disabled successfully");
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getTopTemperature() {
    auto promise = std::make_shared<std::promise<float>>();

    SensorModule::getInstance().getTopTemperature(m_ioContext, [promise](float temperature) {
        promise->set_value(temperature);
    });

    auto future = promise->get_future();
    auto temperature = future.get();

    if (temperature < 0) {
        return createResponse(Status::CODE_500, "Failed to get top sensor temperature");
    }

    auto dto = MyTempDto::createShared();
    dto->temperature = temperature;
    return createDtoResponse(Status::CODE_200, dto);
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::getBottomTemperature() {
    auto promise = std::make_shared<std::promise<float>>();

    SensorModule::getInstance().getBottomTemperature(m_ioContext, [promise](float temperature) {
        promise->set_value(temperature);
    });

    auto future = promise->get_future();
    auto temperature = future.get();

    if (temperature < 0) {
        return createResponse(Status::CODE_500, "Failed to get bottom sensor temperature");
    }

    auto dto = MyTempDto::createShared();
    dto->temperature = temperature;
    return createDtoResponse(Status::CODE_200, dto);
}
