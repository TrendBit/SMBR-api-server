#include "SystemModule.hpp"

SystemModule& SystemModule::getInstance(boost::asio::io_context& io_context, CanRequestManager& canRequestManager) {
    static SystemModule instance(io_context, canRequestManager);
    return instance;
}

SystemModule::SystemModule(boost::asio::io_context& io_context, CanRequestManager& canRequestManager)
    : m_ioContext(io_context), m_canRequestManager(canRequestManager) {}

void SystemModule::getAvailableModules(std::function<void(const std::vector<CanMessage>&)> callback) {
    uint32_t probe_can_id = createCanId(Codes::Message_type::Probe_modules_request, 
                                        Codes::Module::All, 
                                        Codes::Instance::Exclusive, 
                                        false);
    uint32_t probe_response_id = createCanId(Codes::Message_type::Probe_modules_response, 
                                             Codes::Module::All, 
                                             Codes::Instance::Exclusive, 
                                             false);
    std::vector<uint8_t> probe_data = {}; 
    int timeoutSeconds = 2;

    m_canRequestManager.addMultiResponseRequest(probe_can_id, probe_data, probe_response_id, [callback](CanRequestStatus status, const std::vector<CanMessage>& responses) {
        if (status == CanRequestStatus::Success) {
            callback(responses);  
        } else {
            callback({});
        }
    }, timeoutSeconds);
}

