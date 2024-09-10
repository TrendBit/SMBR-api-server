#pragma once

#include "oatpp/macro/component.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "dto/ModuleEnum.hpp"
#include "dto/MyPingResponseDto.hpp"
#include "dto/MyTempDto.hpp"
#include "dto/MyModuleInfoDto.hpp"
#include "dto/MyLoadResponseDto.hpp"
#include "dto/MyModuleActionRequestDto.hpp"
#include "can/CanRequestManager.hpp"
#include "system/SystemModule.hpp"
#include "base/CommonModule.hpp"
#include "oatpp/data/mapping/ObjectMapper.hpp"

#include <future>
#include <iomanip> 

#include OATPP_CODEGEN_BEGIN(ApiController)

/**
 * @class MyController
 * @brief Defines API endpoints for handling system, core, and sensor module actions.
 */
class MyController : public oatpp::web::server::api::ApiController {
public:
    MyController(const std::shared_ptr<oatpp::web::mime::ContentMappers>& apiContentMappers,
                 boost::asio::io_context& ioContext,
                 SystemModule& systemModule,
                 CommonModule& commonModule,
                 CanRequestManager& canRequestManager);

public:

    /**
     * @brief Retrieves available modules and their unique CAN IDs.
     */
    ENDPOINT_INFO(getSystemModules) {
        info->summary = "Retrieves available modules and their respective unique CAN IDs";
        info->addTag("System");
        info->description = "Returns a list of all modules that have responded to the identification message.";
        info->addResponse<List<Object<MyModuleInfoDto>>>(Status::CODE_200, "application/json");
        info->addResponse<String>(Status::CODE_504, "application/json");
    }
    ADD_CORS(getSystemModules)
    ENDPOINT("GET", "/system/modules", getSystemModules);

    /**
     * @brief Sends a ping to the target module and returns the response time.
     */
    ENDPOINT_INFO(ping) {
        info->summary = "Send ping to target module";
        info->addTag("Common");
        info->description = "Sends ping request to target module and waits for response.";
        info->addResponse<Object<MyPingResponseDto>>(Status::CODE_200, "application/json");
        info->addResponse<String>(Status::CODE_500, "application/json");
        info->addResponse<String>(Status::CODE_404, "application/json");
    }
    ADD_CORS(ping)
    ENDPOINT("GET", "/{module}/ping", ping, PATH(oatpp::Enum<dto::ModuleEnum>::AsString, module));

    /**
     * @brief Retrieves the CPU/MCU load and core count of the specified module.
     */
    ENDPOINT_INFO(getLoad) {
        info->summary = "Get module CPU/MCU load";
        info->addTag("Common");
        info->description = "Gets the current workload values of the computing unit, including the average utilization and number of cores.";
        info->addResponse<Object<MyLoadResponseDto>>(Status::CODE_200, "application/json");
        info->addResponse<String>(Status::CODE_404, "application/json", "Module not found");
        info->addResponse<String>(Status::CODE_500, "application/json", "Failed to retrieve load");
        info->addResponse<String>(Status::CODE_504, "application/json", "Request timed out");
    }
    ADD_CORS(getLoad)
    ENDPOINT("GET", "/{module}/load", getLoad, PATH(oatpp::Enum<dto::ModuleEnum>::AsString, module));

    /**
     * @brief Retrieves the CPU/MCU temperature of the specified module.
     */
    ENDPOINT_INFO(getCoreTemp) {
        info->summary = "Get module CPU/MCU temperature";
        info->addTag("Common");
        info->description = "Gets the current temperature of CPU/MCU core values of the computing unit.";
        info->addResponse<Object<MyTempDto>>(Status::CODE_200, "application/json");
        info->addResponse<String>(Status::CODE_404, "application/json", "Module not found");
        info->addResponse<String>(Status::CODE_500, "application/json", "Failed to retrieve temperature");
        info->addResponse<String>(Status::CODE_504, "application/json", "Request timed out");
    }
    ADD_CORS(getCoreTemp)
    ENDPOINT("GET", "/{module}/core_temp", getCoreTemp, PATH(oatpp::Enum<dto::ModuleEnum>::AsString, module));

    /**
     * @brief Restarts the specified module into application mode.
     */
    ENDPOINT_INFO(restartModule) {
        info->summary = "Restart module into application mode";
        info->addTag("Common");
        info->description = "Resets the module and it will start main application firmware again. UID of the module is required.";
        info->addConsumes<Object<MyModuleActionRequestDto>>("application/json");
        info->addResponse(Status::CODE_200, "application/json");
        info->addResponse<String>(Status::CODE_404, "application/json", "Module not found");
        info->addResponse<String>(Status::CODE_500, "application/json", "Failed to restart module");
        info->addResponse<String>(Status::CODE_504, "application/json", "Request timed out");
    }
    ADD_CORS(restartModule)
    ENDPOINT("POST", "/{module}/restart", restartModule,
             PATH(oatpp::Enum<dto::ModuleEnum>::AsString, module),
             BODY_DTO(Object<MyModuleActionRequestDto>, body));


private:
    boost::asio::io_context& m_ioContext;
    CanRequestManager& m_canRequestManager;
    SystemModule& m_systemModule;
    CommonModule& m_commonModule;
};

#include OATPP_CODEGEN_END(ApiController)
