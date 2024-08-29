#include "CommonModule.hpp"
#include "can/CanRequestManager.hpp"
#include "can/CanIdGenerator.hpp"
#include <chrono>
#include <iostream>


CommonModule::CommonModule(Codes::Module mod, Codes::Instance inst)
    : module(mod), instance(inst) {}

CommonModule::~CommonModule() {}

void CommonModule::ping(boost::asio::io_context& io_context, std::function<void(float)> handler) {
    std::cout << "CommonModule::ping - Start" << std::endl;

    uint32_t can_id = createCanId(Codes::Message_type::Ping_request, module, instance, false);
    std::cout << "CommonModule::ping - CAN ID: " << can_id << std::endl;

    uint8_t seq_number = 0x01;
    std::vector<uint8_t> data = {seq_number};
    std::cout << "CommonModule::ping - Data: " << static_cast<int>(data[0]) << std::endl;

    auto start_time = std::chrono::steady_clock::now();
    std::cout << "CommonModule::ping - Start time recorded" << std::endl;

    CanRequestManager::getInstance(io_context).addRequest(can_id, data, 
        [handler, start_time](bool success, const std::vector<uint8_t>&) {
            if (success) {
                auto end_time = std::chrono::steady_clock::now();
                float elapsed_time = std::chrono::duration<float, std::milli>(end_time - start_time).count();
                std::cout << "CommonModule::ping - Success, time: " << elapsed_time << " ms" << std::endl;
                handler(elapsed_time);
            } else {
                std::cout << "CommonModule::ping - Failed" << std::endl;
                handler(-1.0f);
            }
        }
    );
    std::cout << "CommonModule::ping - Request added to CanRequestManager" << std::endl;
}


void CommonModule::getLoad(boost::asio::io_context& io_context, std::function<void(bool, float, int)> handler) {
    uint32_t can_id = createCanId(Codes::Message_type::Core_load_request, module, instance, false);
    std::vector<uint8_t> data = {};

    CanRequestManager::getInstance(io_context).addRequest(can_id, data, 
        [handler](bool success, const std::vector<uint8_t>& response_data) {
            if (success) {
                float load = 20.0f;
                int cores = 4; 
                handler(true, load, cores);
            } else {
                handler(false, 0.0f, 0);
            }
        }
    );
}

void CommonModule::getCoreTemp(boost::asio::io_context& io_context, std::function<void(bool, float)> handler) {
    uint32_t can_id = createCanId(Codes::Message_type::Core_temperature_request, module, instance, false);
    std::vector<uint8_t> data = {};

    CanRequestManager::getInstance(io_context).addRequest(can_id, data, 
        [handler](bool success, const std::vector<uint8_t>& response_data) {
            if (success) {
                float core_temp = 35.0f; 
                handler(true, core_temp);
            } else {
                handler(false, 0.0f);
            }
        }
    );
}

void CommonModule::restart(boost::asio::io_context& io_context, const std::string& uid, std::function<void(bool)> handler) {
    uint32_t can_id = createCanId(Codes::Message_type::Device_reset, module, instance, false);
    std::vector<uint8_t> data(uid.begin(), uid.end());

    CanRequestManager::getInstance(io_context).addRequest(can_id, data, 
        [handler](bool success, const std::vector<uint8_t>&) {
            handler(success);
        }
    );
}

void CommonModule::bootloader(boost::asio::io_context& io_context, const std::string& uid, std::function<void(bool)> handler) {
    uint32_t can_id = createCanId(Codes::Message_type::Device_bootloader, module, instance, false);
    std::vector<uint8_t> data(uid.begin(), uid.end());

    CanRequestManager::getInstance(io_context).addRequest(can_id, data, 
        [handler](bool success, const std::vector<uint8_t>&) {
            handler(success);
        }
    );
}
