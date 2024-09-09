#include "CommonModule.hpp"
#include <iostream>
#include <chrono>

CommonModule::CommonModule(boost::asio::io_context& io_context, CanRequestManager& canRequestManager)
    : m_ioContext(io_context), m_canRequestManager(canRequestManager) {}

void CommonModule::ping(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback) {
    uint32_t ping_can_id = createCanId(Codes::Message_type::Ping_request, module, Codes::Instance::Exclusive, false);
    uint32_t ping_response_id = createCanId(Codes::Message_type::Ping_response, module, Codes::Instance::Exclusive, false);
    std::vector<uint8_t> ping_data = {0x01};  
    int timeoutSeconds = 2;

    auto start_time = std::chrono::steady_clock::now();

    manager.addRequest(ping_can_id, ping_data, ping_response_id, [callback, start_time](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            auto end_time = std::chrono::steady_clock::now();
            float response_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
            callback(response_time_ms); 
        } else if (status == CanRequestStatus::Timeout) {
            callback(-2); 
        } else {
            callback(-1); 
        }
    }, timeoutSeconds);
}



