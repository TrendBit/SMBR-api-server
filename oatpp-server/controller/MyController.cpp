#include "MyController.hpp"
#include <spdlog/spdlog.h>

MyController::MyController(const std::shared_ptr<oatpp::web::mime::ContentMappers>& apiContentMappers,
                           boost::asio::io_context& ioContext,
                           CommonModule& commonModule,
                           CanRequestManager& canRequestManager)
    : oatpp::web::server::api::ApiController(apiContentMappers)
    , m_ioContext(ioContext)
    , m_commonModule(commonModule)
    , m_canRequestManager(canRequestManager) {}

  // ==========================================
  // System Endpoints
  // ==========================================




  // ==========================================
  // Common Endpoints
  // ==========================================

uint8_t MyController::getNextSeqNumber() {
    uint8_t currentSeq = m_seqNum.fetch_add(1);
    if (currentSeq == 255) {
        m_seqNum = 0;  
    }
    return currentSeq;
}

std::optional<Codes::Module> MyController::getTargetModule(const oatpp::Enum<dto::ModuleEnum>::AsString& module) {
    if (module == dto::ModuleEnum::control) {
        return Codes::Module::Control_board;
    } else if (module == dto::ModuleEnum::sensor) {
        return Codes::Module::Sensor_board;
    } else if (module == dto::ModuleEnum::core) {
        return Codes::Module::Core_device;
    }
    return std::nullopt;
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> MyController::ping(const oatpp::Enum<dto::ModuleEnum>::AsString& module) {
    auto pingResponseDto = MyPingResponseDto::createShared();
    std::promise<float> promise;
    auto future = promise.get_future();

    std::atomic<bool> promiseSet = false;
    auto handlePingResult = [&promise, &promiseSet](float responseTime) {
        if (!promiseSet.exchange(true)) {
            promise.set_value(responseTime);
        } else {
            spdlog::warn("Promise already satisfied, ignoring result");
        }
    };

    auto targetModuleOpt = getTargetModule(module);  
    if (!targetModuleOpt.has_value()) {
        return createResponse(Status::CODE_404, "Module not found");  
    }
    Codes::Module targetModule = targetModuleOpt.value();

    uint8_t seq_num = getNextSeqNumber();

    m_commonModule.ping(m_canRequestManager, targetModule, seq_num, handlePingResult);

    future.wait();
    float responseTime = future.get();

    if (responseTime >= 0) {
        pingResponseDto->time_ms = responseTime;
        return createDtoResponse(Status::CODE_200, pingResponseDto);  
    } else if (responseTime == -2) {
        return createResponse(Status::CODE_504, "Ping timed out"); 
    } else {
        return createResponse(Status::CODE_500, "Ping failed"); 
    }
}


