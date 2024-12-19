#include "CommonModule.hpp"
#include <iostream>
#include <iomanip> 
#include <chrono>



CommonModule::CommonModule(boost::asio::io_context& io_context, CanRequestManager& canRequestManager)
    : m_ioContext(io_context), m_canRequestManager(canRequestManager) {}


void CommonModule::ping(CanRequestManager& manager, Codes::Module module, uint8_t seq_num, std::function<void(float)> callback) {
    
    App_messages::Ping_request set_pingReq((uint8_t)seq_num);
    App_messages::Ping_response set_pingRes;
    
    uint32_t ping_can_id = createCanId(set_pingReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t ping_response_id = createCanId(set_pingRes.Type(), module, Codes::Instance::Exclusive, false);
     
    double timeoutSeconds = 1;

    auto start_time = std::chrono::steady_clock::now();

    manager.addRequestWithSeq(ping_can_id, set_pingReq.Export_data(), ping_response_id, seq_num, 
    [callback, start_time](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            auto end_time = std::chrono::steady_clock::now();
            float response_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
            callback(response_time_ms); 
        } else {
            callback((status == CanRequestStatus::Timeout) ? -2 : -1); 
        }
    }, timeoutSeconds);

}


