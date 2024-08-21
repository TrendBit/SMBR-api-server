#include "base/CommonModule.hpp"
#include <chrono>

CommonModule::CommonModule(Codes::Module mod, Codes::Instance inst)
    : module(mod), instance(inst) {}

CommonModule::~CommonModule() {}

uint32_t CommonModule::createCanId(Codes::Message_type messageType, bool emergencyFlag) {
    uint32_t id = 0;
    id |= (emergencyFlag ? 1 : 0) << 28;
    id |= static_cast<uint32_t>(messageType) << 16;
    id |= static_cast<uint32_t>(module) << 4;
    id |= static_cast<uint32_t>(instance);
    return id;
}

float CommonModule::ping() {
    uint8_t seq_number = 0x00;
    uint32_t can_id = createCanId(Codes::Message_type::Ping_request);

    auto start_time = std::chrono::steady_clock::now();

    auto [success, response] = canHandler.sendPingRequest(can_id, seq_number);

    if (!success) {
        return -1.0f;
    }

    do {
        uint32_t receivedId = canHandler.extractCanIdFromResponse(response);

        if ((receivedId & 0xFFF) == static_cast<uint32_t>(Codes::Message_type::Ping_response)) {
            return std::chrono::duration<float, std::milli>(std::chrono::steady_clock::now() - start_time).count();
        }

    } while (canHandler.receiveNextCanMessage(response));

    return -1.0f;
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
