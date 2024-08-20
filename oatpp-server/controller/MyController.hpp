#ifndef MyController_hpp
#define MyController_hpp

#include "oatpp/macro/component.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/macro/codegen.hpp"
#include "dto/MyMessageDto.hpp"
#include "dto/ModuleEnum.hpp" 

#include "core/CoreModule.hpp"
#include "control/ControlModule.hpp"
#include "sensor/SensorModule.hpp"



#include "oatpp/data/mapping/ObjectMapper.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

/**
 * Sample Api Controller.
 */
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
  /**
   * Root endpoint example.
   */
  ENDPOINT_INFO(ping) {
    info->summary = "Send ping to target module";
    info->addResponse<String>(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_500, "application/json");
    info->addResponse<String>(Status::CODE_404, "application/json");

  }

  ENDPOINT("GET", "/{module}/ping", ping,
           PATH(oatpp::Enum<dto::ModuleEnum>::AsString, module)) {
    String response;

    if (module == dto::ModuleEnum::core) {
      CoreModule coreModule;
      response = coreModule.ping();
    } else if (module == dto::ModuleEnum::control) {
      ControlModule controlModule;
      response = controlModule.ping();
    } else if (module == dto::ModuleEnum::sensor) {
      SensorModule sensorModule;
      response = sensorModule.ping();
    } else {
      return createResponse(Status::CODE_404, "Module not found");
    }

    return createResponse(Status::CODE_200, response);
  }

};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* MyController_hpp */