#pragma once

#include "oatpp/macro/component.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/macro/codegen.hpp"
#include <sstream>
#include <iostream>
#include <iomanip>

#include "dto/ModuleEnum.hpp"
#include "dto/MyModuleInfoDto.hpp"
#include "system/SystemModule.hpp"
#include "oatpp/data/mapping/ObjectMapper.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

/**
 * @class MyController
 * @brief Controller class handling the API endpoints for system, core, control, and sensor modules.
 * 
 * The MyController class defines the REST API endpoints used to interact with the various modules of the system.
 * It includes endpoints for retrieving system information, controlling the heater, and getting temperature readings.
 */
class MyController : public oatpp::web::server::api::ApiController {

public:
  /**
   * @brief Constructor for MyController.
   * 
   * Initializes the controller with the provided content mappers for JSON serialization and deserialization.
   * 
   * @param apiContentMappers - Mappers used for serializing and deserializing DTOs.
   * @param ioContext - Reference to the Boost ASIO io_context.
   * @param systemModule - Reference to the SystemModule instance.
   */
  MyController(const std::shared_ptr<oatpp::web::mime::ContentMappers>& apiContentMappers,
               boost::asio::io_context& ioContext,
               SystemModule& systemModule)
    : oatpp::web::server::api::ApiController(apiContentMappers)
    , m_ioContext(ioContext)
    , m_systemModule(systemModule)
  {}

public:

  /**
   * @brief Endpoint that retrieves available modules and their respective unique CAN IDs.
   * 
   * This endpoint sends a request to probe the modules on the system and returns their IDs and types.
   * 
   * @return A list of available modules, including their type and unique CAN ID.
   */
  ENDPOINT_INFO(getSystemModules) {
    info->summary = "Retrieves available modules and their respective unique CAN IDs";
    info->addTag("System");
    info->description = "Returns a list of all modules that have responded to the identification message.";
    info->addResponse<List<Object<MyModuleInfoDto>>>(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_504, "application/json");
  }
  ADD_CORS(getSystemModules)
  
  /**
   * @brief Handles the GET request for retrieving the available modules.
   * 
   * This function processes the response from the CAN bus to retrieve module IDs and types,
   * and then returns the result in JSON format. In case of a timeout, it returns a 504 status.
   * 
   * @return A JSON response containing module details or a timeout message.
   */
  ENDPOINT("GET", "/system/modules", getSystemModules) {
    auto dtoList = oatpp::List<oatpp::Object<MyModuleInfoDto>>::createShared();  

    std::shared_ptr<std::promise<oatpp::List<oatpp::Object<MyModuleInfoDto>>>> promise = std::make_shared<std::promise<oatpp::List<oatpp::Object<MyModuleInfoDto>>>>();
    std::future<oatpp::List<oatpp::Object<MyModuleInfoDto>>> future = promise->get_future();

    m_systemModule.getAvailableModules([promise, dtoList, this](const std::vector<CanMessage>& responses) {
      if (!responses.empty()) {
        std::cout << "Received " << responses.size() << " module responses:" << std::endl;

        for (const auto& response : responses) {
          auto moduleInfoDto = MyModuleInfoDto::createShared();

          std::stringstream uidHex;
          uidHex << "0x";
          for (const auto& byte : response.getData()) {
            uidHex << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte);
          }
          moduleInfoDto->uid = uidHex.str();

          std::cout << "Module response with ID: 0x" << std::hex << response.getId() << " and data: " << uidHex.str() << std::endl;

          uint32_t modulePart = (response.getId() >> 4) & 0xFF;
          switch(static_cast<Codes::Module>(modulePart)) {
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

        promise->set_value(dtoList);

      } else {
        std::cerr << "No module responses received or request failed. Timeout occurred." << std::endl;

        auto emptyList = oatpp::List<oatpp::Object<MyModuleInfoDto>>::createShared();
        promise->set_value(emptyList);
      }
    });

    future.wait();
    oatpp::List<oatpp::Object<MyModuleInfoDto>> result = future.get();

    if (result->size() == 0) {
      return createResponse(Status::CODE_504, "No module responses received (timeout)");
    }

    return createDtoResponse(Status::CODE_200, result);  
  }

private:
  boost::asio::io_context& m_ioContext;  
  SystemModule& m_systemModule;  

};

#include OATPP_CODEGEN_END(ApiController)
