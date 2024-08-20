#ifndef MyController_hpp
#define MyController_hpp

#include "oatpp/macro/component.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/macro/codegen.hpp"
#include "dto/ModuleEnum.hpp"
#include "dto/MyPingResponseDto.hpp"

#include "core/CoreModule.hpp"
#include "control/ControlModule.hpp"
#include "sensor/SensorModule.hpp"

#include "oatpp/data/mapping/ObjectMapper.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

class MyController : public oatpp::web::server::api::ApiController {
public:
  /**
   * Constructor with object mapper.
   * @param apiContentMappers - mappers used to serialize/deserialize DTOs.
   */
  MyController(OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>, apiContentMappers))
    : oatpp::web::server::api::ApiController(apiContentMappers)
  {}

public:
  ENDPOINT_INFO(ping) {
    info->summary = "Send ping to target module";
    info->addResponse<Object<MyPingResponseDto>>(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_500, "application/json");
    info->addResponse<String>(Status::CODE_404, "application/json");
    info->addResponse<String>(Status::CODE_408, "application/json");  
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

    if (responseTime == -1.0f) {
      return createResponse(Status::CODE_408, "Ping response timeout"); 
    } else if (responseTime < 0) {
      return createResponse(Status::CODE_500, "Ping failed");
    }

    auto dto = MyPingResponseDto::createShared();
    dto->time_ms = responseTime;

    return createDtoResponse(Status::CODE_200, dto);
  }

};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* MyController_hpp */
