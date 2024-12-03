#pragma once

#include "oatpp/macro/component.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "dto/ModuleEnum.hpp"
#include "dto/MyPingResponseDto.hpp"
#include "dto/MyTempDto.hpp"
#include "dto/MyModuleInfoDto.hpp"
#include "dto/MyLoadResponseDto.hpp"
#include "dto/MyModuleActionRequestDto.hpp"
#include "dto/MyIntensityDto.hpp"
#include "can/CanRequestManager.hpp"
#include "system/SystemModule.hpp"
#include "base/CommonModule.hpp"
#include "control/ControlModule.hpp"
#include "oatpp/data/mapping/ObjectMapper.hpp"


#include <future>
#include <iomanip>
#include <cstdint>
#include <utility>
#include <unordered_set>
#include <optional>

#include <iostream>

#include OATPP_CODEGEN_BEGIN(ApiController)

/**
 * @class MyController
 * @brief Defines API endpoints for handling system, core, and sensor module actions.
 */
class MyController : public oatpp::web::server::api::ApiController {
public:
    MyController(const std::shared_ptr<oatpp::web::mime::ContentMappers>& apiContentMappers,
                 boost::asio::io_context& ioContext,
                 CommonModule& commonModule,
                 CanRequestManager& canRequestManager);

public:


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
    * @brief Retrieves the CPU/MCU load of the specified module.
    */

private:
    /**
     * @brief Increments the sequence number and resets to 0 after 255.
     */
    uint8_t getNextSeqNumber();

    std::optional<Codes::Module> getTargetModule(const oatpp::Enum<dto::ModuleEnum>::AsString& module);

    boost::asio::io_context& m_ioContext;
    CommonModule& m_commonModule;
    CanRequestManager& m_canRequestManager;
    std::atomic<uint8_t> m_seqNum{0};  
};

#include OATPP_CODEGEN_END(ApiController)
