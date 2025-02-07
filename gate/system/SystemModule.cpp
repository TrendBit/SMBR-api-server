#include "SystemModule.hpp"

extern backward::SignalHandling sh;

SystemModule& SystemModule::getInstance(boost::asio::io_context& io_context, CanRequestManager& canRequestManager) {
    static SystemModule instance(io_context, canRequestManager);
    return instance;
}

SystemModule::SystemModule(boost::asio::io_context& io_context, CanRequestManager& canRequestManager)
    : m_ioContext(io_context), m_canRequestManager(canRequestManager) {}

void SystemModule::getAvailableModules(std::function<void(const std::vector<CanMessage>&)> callback) {

    App_messages::Common::Probe_modules_request set_moduleReq;
    App_messages::Common::Probe_modules_response set_moduleRes;

    uint32_t probe_can_id = createCanId(set_moduleReq.Type(), 
                                        Codes::Module::All, 
                                        Codes::Instance::Exclusive, 
                                        false);
    uint32_t probe_response_id = createCanId(set_moduleRes.Type(), 
                                             Codes::Module::All, 
                                             Codes::Instance::Exclusive, 
                                             false);
   
    double timeoutSeconds = 0.3;

    m_canRequestManager.addMultiResponseRequest(probe_can_id, set_moduleReq.Export_data(), probe_response_id, [callback](CanRequestStatus status, const std::vector<CanMessage>& responses) {
        if (status == CanRequestStatus::Success) {
            callback(responses);  
        } else {
            callback({});
        }
    }, timeoutSeconds);
}

