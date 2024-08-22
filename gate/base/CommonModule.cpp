#include "base/CommonModule.hpp"
#include "can/CanHandler.hpp"
#include "can/CanUtils.hpp"  
#include <chrono>

CommonModule::CommonModule(Codes::Module mod, Codes::Instance inst)
    : module(mod), instance(inst) {}

CommonModule::~CommonModule() {}

float CommonModule::ping() {
    uint8_t seq_number = 0x00;
    uint32_t can_id = createCanId(Codes::Message_type::Ping_request, module, instance);

    auto start_time = std::chrono::steady_clock::now();

    CanHandler canHandler;
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
    uint32_t can_id = createCanId(Codes::Message_type::Core_load_request, module, instance);
    
    CanHandler canHandler;
    canHandler.sendLoadRequest(can_id);  

    return 0.0f;
}

int CommonModule::getCoreCount() {
    return 4; 
}

float CommonModule::getCoreTemperature() {
    uint32_t can_id = createCanId(Codes::Message_type::Core_temperature_request, module, instance);
    
    CanHandler canHandler;
    canHandler.sendTemperatureRequest(can_id);  

    return 0.0f; 
}

bool CommonModule::restart(const std::string& uid) {
    uint32_t can_id = createCanId(Codes::Message_type::Device_reset, module, instance);
    
    CanHandler canHandler;
    return canHandler.sendRestartRequest(can_id, uid);  
}

bool CommonModule::bootloader(const std::string& uid) {
    uint32_t can_id = createCanId(Codes::Message_type::Device_bootloader, module, instance);
    
    CanHandler canHandler;
    return canHandler.sendBootloaderRequest(can_id, uid);  
}
