#ifndef MyController_hpp
#define MyController_hpp

#include "oatpp/macro/component.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/macro/codegen.hpp"

#include "dto/ModuleEnum.hpp"
#include "dto/MyPingResponseDto.hpp"
#include "dto/MyLoadResponseDto.hpp"
#include "dto/MyCoreTempResponseDto.hpp" 
#include "dto/MyModuleActionRequestDto.hpp" 
#include "dto/MyModuleInfoDto.hpp" 
#include "dto/MySupplyTypeResponseDto.hpp" 
#include "dto/MyTempDto.hpp" 

#include "core/CoreModule.hpp"
#include "control/ControlModule.hpp"
#include "sensor/SensorModule.hpp"
#include "system/SystemModule.hpp"

#include "oatpp/data/mapping/ObjectMapper.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

class MyController : public oatpp::web::server::api::ApiController {

private:
    SystemModule systemModule;

public:
  /**
   * Constructor with object mapper.
   * @param apiContentMappers - mappers used to serialize/deserialize DTOs.
   */
  MyController(OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>, apiContentMappers))
    : oatpp::web::server::api::ApiController(apiContentMappers)
  {}

public:

ENDPOINT("GET", "/{module}/ping", ping,
           PATH(oatpp::Enum<dto::ModuleEnum>::AsString, module)) {

    float responseTime = -1.0f;

    if (module == dto::ModuleEnum::control) {
        ControlModule& controlModule = ControlModule::getInstance();
        responseTime = controlModule.ping();
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
// ==========================================
// System Endpoints
// ==========================================
/*
ENDPOINT_INFO(getAvailableModules) {
        info->summary = "Determines which all modules are available on the device and their respective unique CAN IDs";
        info->addTag("System");
        info->description = "Returns a list of all modules that have responded to the identification message and can therefore be considered available on the device.";
        info->addResponse<List<Object<MyModuleInfoDto>>>(Status::CODE_200, "application/json");
    }

    ENDPOINT("GET", "/system/modules", getAvailableModules) {
        auto modules = systemModule.getAvailableModules(); 
        return createDtoResponse(Status::CODE_200, modules);
    }

    ENDPOINT_INFO(getSystemTemperature) {
        info->summary = "Get the main temperature of the system/bottle";
        info->addTag("System");
        info->description = "This temperature could be obtained by different means based on configuration. Configured module (sensor module at default) will periodically calculate temperature of system and will it broadcast it via CAN. On side of gateway this last temperature value is saved and supplied to API.";
        info->addResponse<Object<MyTempDto>>(Status::CODE_200, "application/json");
    }

    ENDPOINT("GET", "/system/temperature", getSystemTemperature) {
        auto dto = systemModule.getSystemTemperature(); 
        return createDtoResponse(Status::CODE_200, dto);
    }


// ==========================================
// Common Endpoints
// ==========================================

  ENDPOINT_INFO(ping) {
    info->summary = "Send ping to target module";
    info->addTag("Common");
    info->description = "Sends ping request to target module and wait for response. If response is not received in 1 seconds, then timeouts.";
    info->addResponse<Object<MyPingResponseDto>>(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_500, "application/json");
    info->addResponse<String>(Status::CODE_404, "application/json");
  }

  ENDPOINT("GET", "/{module}/ping", ping,
           PATH(oatpp::Enum<dto::ModuleEnum>::AsString, module)) {

    float responseTime = -1.0f;

    if (module == dto::ModuleEnum::core) {
      CoreModule coreModule;
      responseTime = coreModule.ping();
    } else if (module == dto::ModuleEnum::control) {
      ControlModule controlModule;
      responseTime = controlModule.ping();
    } else if (module == dto::ModuleEnum::sensor) {
      SensorModule sensorModule;
      responseTime = sensorModule.ping();
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

ENDPOINT_INFO(getModuleLoad) {
    info->summary = "Get module CPU/MCU load";
    info->addTag("Common");
    info->description = "Gets the current workload values of the computing unit, including the average utilization and number of cores.";
    info->addResponse<Object<MyLoadResponseDto>>(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "application/json", "Module not found");
}

ENDPOINT("GET", "/{module}/load", getModuleLoad,
         PATH(oatpp::Enum<dto::ModuleEnum>::AsString, module)) {

    float loadValue = -1.0f;
    int cores = 0;

    if (module == dto::ModuleEnum::core) {
        CoreModule coreModule;
        loadValue = coreModule.getLoad();
        cores = coreModule.getCoreCount();
    } else if (module == dto::ModuleEnum::control) {
        ControlModule controlModule;
        loadValue = controlModule.getLoad();
        cores = controlModule.getCoreCount();
    } else if (module == dto::ModuleEnum::sensor) {
        SensorModule sensorModule;
        loadValue = sensorModule.getLoad();
        cores = sensorModule.getCoreCount();
    } else {
        return createResponse(Status::CODE_404, "Module not found");
    }

    if (loadValue < 0) {
        return createResponse(Status::CODE_500, "Failed to retrieve load");
    }

    auto dto = MyLoadResponseDto::createShared();
    dto->load = loadValue;
    dto->cores = cores;

    return createDtoResponse(Status::CODE_200, dto);
}

ENDPOINT_INFO(getModuleCoreTemperature) {
    info->summary = "Get module CPU/MCU temperature";
    info->addTag("Common");
    info->description = "Gets the current temperature of CPU/MCU core values of the computing unit.";
    info->addResponse<Object<MyCoreTempResponseDto>>(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "application/json", "Module not found");
}

ENDPOINT("GET", "/{module}/core_temp", getModuleCoreTemperature,
         PATH(oatpp::Enum<dto::ModuleEnum>::AsString, module)) {

    float coreTemp = -1.0f;

    if (module == dto::ModuleEnum::core) {
        CoreModule coreModule;
        coreTemp = coreModule.getCoreTemperature();
    } else if (module == dto::ModuleEnum::control) {
        ControlModule controlModule;
        coreTemp = controlModule.getCoreTemperature();
    } else if (module == dto::ModuleEnum::sensor) {
        SensorModule sensorModule;
        coreTemp = sensorModule.getCoreTemperature();
    } else {
        return createResponse(Status::CODE_404, "Module not found");
    }

    if (coreTemp < 0) {
        return createResponse(Status::CODE_500, "Failed to retrieve core temperature");
    }

    auto dto = MyCoreTempResponseDto::createShared();
    dto->core_temp = coreTemp;

    return createDtoResponse(Status::CODE_200, dto);
}

ENDPOINT_INFO(restartModule) {
    info->summary = "Restart module into application mode";
    info->addTag("Common");
    info->addConsumes<Object<MyModuleActionRequestDto>>("application/json");
    info->addResponse(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "application/json");
}

ENDPOINT("POST", "/{module}/restart", restartModule,
         PATH(oatpp::Enum<dto::ModuleEnum>::AsString, module),
         BODY_DTO(Object<MyModuleActionRequestDto>, body)) {

    bool success = false;

    std::string uid = std::to_string(body->uid);  

    if (module == dto::ModuleEnum::core) {
        CoreModule coreModule;
        success = coreModule.restart(uid);
    } else if (module == dto::ModuleEnum::control) {
        ControlModule controlModule;
        success = controlModule.restart(uid);
    } else if (module == dto::ModuleEnum::sensor) {
        SensorModule sensorModule;
        success = sensorModule.restart(uid);
    } else {
        return createResponse(Status::CODE_404, "Module not found");
    }

    if (success) {
        return createResponse(Status::CODE_200, "Successfully restarted module");
    } else {
        return createResponse(Status::CODE_500, "Failed to restart module");
    }
}

ENDPOINT_INFO(restartModuleBootloader) {
    info->summary = "Reboot module in bootloader (katapult) mode";
    info->addTag("Common");
    info->addConsumes<Object<MyModuleActionRequestDto>>("application/json");
    info->addResponse(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "application/json");
}

ENDPOINT("POST", "/{module}/bootloader", restartModuleBootloader,
         PATH(oatpp::Enum<dto::ModuleEnum>::AsString, module),
         BODY_DTO(Object<MyModuleActionRequestDto>, body)) {

    bool success = false;

    std::string uid = std::to_string(body->uid);  

    if (module == dto::ModuleEnum::core) {
        CoreModule coreModule;
        success = coreModule.bootloader(uid);
    } else if (module == dto::ModuleEnum::control) {
        ControlModule controlModule;
        success = controlModule.bootloader(uid);
    } else if (module == dto::ModuleEnum::sensor) {
        SensorModule sensorModule;
        success = sensorModule.bootloader(uid);
    } else {
        return createResponse(Status::CODE_404, "Module not found");
    }

    if (success) {
        return createResponse(Status::CODE_200, "Successfully restarted module in bootloader mode");
    } else {
        return createResponse(Status::CODE_500, "Failed to restart module in bootloader mode");
    }
}


// ==========================================
// Core module
// ==========================================
ENDPOINT_INFO(getSupplyType) {
    info->summary = "Determine which power supply is currently in use (Adapter/PoE+)";
    info->addTag("Core module");
    info->description = "Device can be powered from two power sources (AC adapter -> 12V or PoE+). This endpoint determines which supply is connected.";
    info->addResponse<Object<MySupplyTypeResponseDto>>(Status::CODE_200, "application/json");
}

ENDPOINT("GET", "/core/supply_type", getSupplyType) {

    CoreModule coreModule;
    auto dto = coreModule.getSupplyType();

    return createDtoResponse(Status::CODE_200, dto);
}

// ==========================================
// Control module
// ==========================================
ENDPOINT_INFO(setHeaterTemperature) {
    info->summary = "Set the target bottle heating temperature";
    info->addTag("Control module");
    info->description = "Set target temperature for bottle heater which should be reached and maintained.";
    info->addConsumes<Object<MyTempDto>>("application/json");
    info->addResponse(Status::CODE_200, "application/json");
}

ENDPOINT("POST", "/control/heater", setHeaterTemperature,
         BODY_DTO(Object<MyTempDto>, heaterDto)) {

    ControlModule controlModule;
    bool success = controlModule.setHeaterTemperature(heaterDto->temperature);

    if (success) {
        return createResponse(Status::CODE_200, "Heater target temperature set successfully");
    } else {
        return createResponse(Status::CODE_500, "Failed to set heater target temperature");
    }
}

ENDPOINT_INFO(getHeaterTemperature) {
    info->summary = "Get the current target heating temperature";
    info->addTag("Control module");
    info->description = "Get current target temperature of bottle heater.";
    info->addResponse<Object<MyTempDto>>(Status::CODE_200, "application/json");
}

ENDPOINT("GET", "/control/heater", getHeaterTemperature) {

    ControlModule controlModule;
    auto dto = controlModule.getHeaterTemperature();

    return createDtoResponse(Status::CODE_200, dto);
}

ENDPOINT_INFO(disableHeater) {
    info->summary = "Disables the bottle heater";
    info->addTag("Control module");
    info->description = "Disables the bottle heater, this will stop the heating process.";
    info->addResponse(Status::CODE_200, "application/json");
}

ENDPOINT("GET", "/control/disable_heater", disableHeater) {

    ControlModule controlModule;
    bool success = controlModule.disableHeater();

    if (success) {
        return createResponse(Status::CODE_200, "Heater disabled successfully");
    } else {
        return createResponse(Status::CODE_500, "Failed to disable heater");
    }
}

// ==========================================
// Sensor module
// ==========================================
ENDPOINT_INFO(getTopSensorTemperature) {
    info->summary = "Get the temperature of the top sensor on cultivation bottle";
    info->addTag("Sensor module");
    info->description = "Get the current temperature of the top sensor on cultivation bottle.";
    info->addResponse<Object<MyTempDto>>(Status::CODE_200, "application/json");
}

ENDPOINT("GET", "/sensor/temperature_top", getTopSensorTemperature) {
    SensorModule sensorModule;
    auto dto = sensorModule.getTopSensorTemperature(); 

    return createDtoResponse(Status::CODE_200, dto);
}

ENDPOINT_INFO(getBottomSensorTemperature) {
    info->summary = "Get the temperature of the bottom sensor on cultivation bottle";
    info->addTag("Sensor module");
    info->description = "Get the current temperature of the bottom sensor on cultivation bottle.";
    info->addResponse<Object<MyTempDto>>(Status::CODE_200, "application/json");
}

ENDPOINT("GET", "/sensor/temperature_bottom", getBottomSensorTemperature) {
    SensorModule sensorModule;
    auto dto = sensorModule.getBottomSensorTemperature(); 

    return createDtoResponse(Status::CODE_200, dto);
}
*/


};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* MyController_hpp */