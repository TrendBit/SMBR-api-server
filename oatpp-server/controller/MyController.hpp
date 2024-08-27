#ifndef MyController_hpp
#define MyController_hpp

#include "oatpp/macro/component.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/macro/codegen.hpp"
#include <sstream>

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

/**
 * @class MyController
 * @brief Controller class handling the API endpoints for system, core, control, and sensor modules.
 */
class MyController : public oatpp::web::server::api::ApiController {

public:
  /**
   * @brief Constructor for MyController.
   * @param apiContentMappers - Mappers used for serializing and deserializing DTOs.
   */
  MyController(OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>, apiContentMappers))
    : oatpp::web::server::api::ApiController(apiContentMappers)
  {}

public:

  // ==========================================
  // System Endpoints
  // ==========================================
  ENDPOINT_INFO(getSystemModules) {
    info->summary = "Determines which all modules are available on the device and their respective unique CAN IDs";
    info->addTag("System");
    info->description = "Returns a list of all modules that have responded to the identification message and can therefore be considered available on the device.";
    info->addResponse<List<Object<MyModuleInfoDto>>>(Status::CODE_200, "application/json");
  }
  ADD_CORS(getSystemModules)
  ENDPOINT("GET", "/system/modules", getSystemModules);

  ENDPOINT_INFO(getSystemTemperature) {
    info->summary = "Get the main temperature of the system/bottle";
    info->addTag("System");
    info->description = "This temperature could be obtained by different means based on configuration. Configured module (sensor module at default) will periodically calculate temperature of system and will it broadcast it via CAN. On side of gateway this last temperature value is saved and supplied to API.";
    info->addResponse<Object<MyTempDto>>(Status::CODE_200, "application/json");
  }
  ADD_CORS(getSystemTemperature)
  ENDPOINT("GET", "/system/temperature", getSystemTemperature);

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
  ADD_CORS(ping)
  ENDPOINT("GET", "/{module}/ping", ping, PATH(oatpp::Enum<dto::ModuleEnum>::AsString, module));

  ENDPOINT_INFO(getLoad) {
    info->summary = "Get module CPU/MCU load";
    info->addTag("Common");
    info->description = "Gets the current workload values of the computing unit, including the average utilization and number of cores.";
    info->addResponse<Object<MyLoadResponseDto>>(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "application/json", "Module not found");
  }
  ADD_CORS(getLoad)
  ENDPOINT("GET", "/{module}/load", getLoad, PATH(oatpp::Enum<dto::ModuleEnum>::AsString, module));

  ENDPOINT_INFO(getCoreTemp) {
    info->summary = "Get module CPU/MCU temperature";
    info->addTag("Common");
    info->description = "Gets the current temperature of CPU/MCU core values of the computing unit.";
    info->addResponse<Object<MyCoreTempResponseDto>>(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "application/json", "Module not found");
  }
  ADD_CORS(getCoreTemp)
  ENDPOINT("GET", "/{module}/core_temp", getCoreTemp, PATH(oatpp::Enum<dto::ModuleEnum>::AsString, module));

  ENDPOINT_INFO(restartModule) {
    info->summary = "Restart module into application mode";
    info->addTag("Common");
    info->addConsumes<Object<MyModuleActionRequestDto>>("application/json");
    info->addResponse(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "application/json");
  }
  ADD_CORS(restartModule)
  ENDPOINT("POST", "/{module}/restart", restartModule,
           PATH(oatpp::Enum<dto::ModuleEnum>::AsString, module),
           BODY_DTO(Object<MyModuleActionRequestDto>, body));

  ENDPOINT_INFO(bootloaderModule) {
    info->summary = "Reboot module in bootloader (katapult) mode";
    info->addTag("Common");
    info->addConsumes<Object<MyModuleActionRequestDto>>("application/json");
    info->addResponse(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "application/json");
  }
  ADD_CORS(bootloaderModule)
  ENDPOINT("POST", "/{module}/bootloader", bootloaderModule,
           PATH(oatpp::Enum<dto::ModuleEnum>::AsString, module),
           BODY_DTO(Object<MyModuleActionRequestDto>, body));

  // ==========================================
  // Core module
  // ==========================================
  ENDPOINT_INFO(getSupplyType) {
    info->summary = "Determine which power supply is currently in use (Adapter/PoE+)";
    info->addTag("Core module");
    info->description = "Device can be powered from two power sources (AC adapter -> 12V or PoE+). This endpoint determines which supply is connected.";
    info->addResponse<Object<MySupplyTypeResponseDto>>(Status::CODE_200, "application/json");
  }
  ADD_CORS(getSupplyType)
  ENDPOINT("GET", "/core/supply_type", getSupplyType);

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
  ADD_CORS(setHeaterTemperature)
  ENDPOINT("POST", "/control/heater", setHeaterTemperature,
           BODY_DTO(Object<MyTempDto>, body));

  ENDPOINT_INFO(getHeaterTemperature) {
    info->summary = "Get the current target heating temperature";
    info->addTag("Control module");
    info->description = "Get current target temperature of bottle heater.";
    info->addResponse<Object<MyTempDto>>(Status::CODE_200, "application/json");
  }
  ADD_CORS(getHeaterTemperature)
  ENDPOINT("GET", "/control/heater", getHeaterTemperature);

  ENDPOINT_INFO(disableHeater) {
    info->summary = "Disables the bottle heater";
    info->addTag("Control module");
    info->description = "Disables the bottle heater, this will stop the heating process.";
    info->addResponse(Status::CODE_200, "application/json");
  }
  ADD_CORS(disableHeater)
  ENDPOINT("GET", "/control/disable_heater", disableHeater);

  // ==========================================
  // Sensor module
  // ==========================================
  ENDPOINT_INFO(getTopTemperature) {
    info->summary = "Get the temperature of the top sensor on cultivation bottle";
    info->addTag("Sensor module");
    info->description = "Get the current temperature of the top sensor on cultivation bottle.";
    info->addResponse<Object<MyTempDto>>(Status::CODE_200, "application/json");
  }
  ADD_CORS(getTopTemperature)
  ENDPOINT("GET", "/sensor/temperature_top", getTopTemperature);

  ENDPOINT_INFO(getBottomTemperature) {
    info->summary = "Get the temperature of the bottom sensor on cultivation bottle";
    info->addTag("Sensor module");
    info->description = "Get the current temperature of the bottom sensor on cultivation bottle.";
    info->addResponse<Object<MyTempDto>>(Status::CODE_200, "application/json");
  }
  ADD_CORS(getBottomTemperature)
  ENDPOINT("GET", "/sensor/temperature_bottom", getBottomTemperature);

};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* MyController_hpp */
