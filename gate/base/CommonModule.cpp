#include "base/CommonModule.hpp"
#include <chrono>
#include <iostream>
#include <future>

CommonModule::CommonModule(Codes::Module mod, Codes::Instance inst)
    : module(mod), instance(inst) {}

CommonModule::~CommonModule() {}

uint32_t CommonModule::createCanId(Codes::Message_type messageType, bool emergencyFlag) {
    uint32_t id = 0;
    id |= (emergencyFlag ? 1 : 0) << 28;
    id |= static_cast<uint32_t>(messageType) << 16;
    id |= static_cast<uint32_t>(module) << 4;
    id |= static_cast<uint32_t>(instance);

    std::cout << "Created CAN ID: 0x" << std::hex << id << std::dec << std::endl;
    std::cout << " - Emergency flag: " << (emergencyFlag ? "set" : "not set") << std::endl;
    std::cout << " - Message Type: 0x" << std::hex << static_cast<uint32_t>(messageType) << std::dec << std::endl;
    std::cout << " - Module: 0x" << std::hex << static_cast<uint32_t>(module) << std::dec << std::endl;
    std::cout << " - Instance: 0x" << std::hex << static_cast<uint32_t>(instance) << std::dec << std::endl;

    return id;
}

float CommonModule::ping() {
    uint8_t seq_number = 0x00;
    uint32_t can_id = createCanId(Codes::Message_type::Ping_request);

    auto start_time = std::chrono::steady_clock::now();
    auto timeout_duration = std::chrono::seconds(3);  

    auto [success, response] = canHandler.sendPingRequest(can_id, seq_number);

    if (!success) {
        std::cerr << "Failed to send CAN message." << std::endl;
        return -1.0f;
    }


    auto future_response = std::async(std::launch::async, [&] {
        while (canHandler.receiveNextCanMessage(response)) {
            uint32_t receivedId = canHandler.extractCanIdFromResponse(response);

            if ((receivedId & 0xFFF) == static_cast<uint32_t>(Codes::Message_type::Ping_response)) {
                return std::chrono::duration<float, std::milli>(std::chrono::steady_clock::now() - start_time).count();
            }
        }
        return -1.0f; 
    });


    if (future_response.wait_for(timeout_duration) == std::future_status::timeout) {
        std::cerr << "Ping response timeout after 3 seconds." << std::endl;
        return -1.0f;
    } else {
        return future_response.get(); 
    }
}



float CommonModule::getLoad() {
    return 0.0;
}

float CommonModule::getCoreTemperature() {
    return 0.0;
}

bool CommonModule::restart(const std::string& uid) {
    return false;
}

bool CommonModule::bootloader(const std::string& uid) {
    return false;
}
