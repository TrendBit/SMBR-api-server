#ifndef MyController_hpp
#define MyController_hpp

#include "oatpp/macro/component.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/macro/codegen.hpp"
#include "dto/MyMessageDto.hpp"
#include "dto/MyTemperatureDto.hpp"
#include "MyCustomApi.h"

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
    : oatpp::web::server::api::ApiController(apiContentMappers), apiClient() 
  {}

public:
  /**
   * Root endpoint example.
   */
  ENDPOINT_INFO(root) {
    info->summary = "Root endpoint, returns 'Hello World!'";
    info->addResponse<oatpp::Object<MyMessageDto>>(Status::CODE_200, "application/json");
  }
  ENDPOINT("GET", "/", root) {
    auto dto = MyMessageDto::createShared();
    dto->statusCode = 200;
    dto->message = "Hello World!";
    return createDtoResponse(Status::CODE_200, dto);
  }

  /**
  * Ping endpoint to send CAN message and get response.
  */
  ENDPOINT_INFO(ping) {
    info->summary = "Ping endpoint to send CAN message and get response";
    info->addResponse<oatpp::Object<MyMessageDto>>(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_500, "application/json");
  }

  ENDPOINT("GET", "/ping", ping) {
    auto dto = MyMessageDto::createShared();
    dto->statusCode = 200;
    std::string response = apiClient.ping();
    if (response == "CAN message failed") {
      dto->statusCode = 500;
      dto->message = response;
      return createDtoResponse(Status::CODE_500, dto);
    }
    dto->message = response;
    return createDtoResponse(Status::CODE_200, dto);
  }



private:
  OpenAPI::MyCustomApi apiClient;
};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* MyController_hpp */