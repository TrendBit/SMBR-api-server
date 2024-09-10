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

void CommonModule::getLoad(CanRequestManager& manager, Codes::Module module, std::function<void(float, int)> callback) {
    uint32_t load_can_id = createCanId(Codes::Message_type::Core_load_request, module, Codes::Instance::Exclusive, false);
    uint32_t load_response_id = createCanId(Codes::Message_type::Core_load_response, module, Codes::Instance::Exclusive, false);
    std::vector<uint8_t> load_data = {};  
    int timeoutSeconds = 1.5;

    manager.addRequest(load_can_id, load_data, load_response_id, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            if (response.getData().size() >= 8) {
                float load = *reinterpret_cast<const float*>(response.getData().data());
                int cores = *reinterpret_cast<const int*>(response.getData().data() + 4);
                callback(load, cores);  
            } else {
                callback(-1, 0);  
            }
        } else if (status == CanRequestStatus::Timeout) {
            callback(-2, 0);  
        } else {
            callback(-1, 0); 
        }
    }, timeoutSeconds);
}

void CommonModule::getCoreTemp(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback) {
    uint32_t temp_can_id = createCanId(Codes::Message_type::Core_temperature_request, module, Codes::Instance::Exclusive, false);
    uint32_t temp_response_id = createCanId(Codes::Message_type::Core_temperature_response, module, Codes::Instance::Exclusive, false);
    std::vector<uint8_t> temp_data = {};  
    int timeoutSeconds = 3;

    manager.addRequest(temp_can_id, temp_data, 0x203, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            if (response.getData().size() >= 4) {
                float temperature = *reinterpret_cast<const float*>(response.getData().data());
                callback(temperature);  
            } else {
                callback(-1);  
            }
        } else if (status == CanRequestStatus::Timeout) {
            callback(-2);  
        } else {
            callback(-1); 
        }
    }, timeoutSeconds);
}

void CommonModule::restartModule(CanRequestManager& manager, Codes::Module module, const std::string& uid, std::function<void(bool)> callback) {
    uint32_t restart_can_id = createCanId(Codes::Message_type::Device_reset, module, Codes::Instance::Exclusive, false);
    std::vector<uint8_t> restart_data(8, 0x00); 

    for (size_t i = 0; i < std::min(uid.size(), static_cast<size_t>(8)); ++i) {
        restart_data[i] = static_cast<uint8_t>(uid[i]);
    }

    int timeoutSeconds = 3;

    manager.addRequest(restart_can_id, restart_data, 0x200, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            callback(true);  
        } else {
            callback(false); 
        }
    }, timeoutSeconds);
}






