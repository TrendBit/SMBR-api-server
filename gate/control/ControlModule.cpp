#include "ControlModule.hpp"

ControlModule::ControlModule() 
    : CommonModule(Codes::Module::Control_board, Codes::Instance::Exclusive) {}

ControlModule& ControlModule::getInstance() {
    static ControlModule instance;
    return instance;
}

bool ControlModule::setHeaterTemperature(float temperature) {
    uint32_t can_id = createCanId(Codes::Message_type::Undefined, module, instance, false);
    
    std::vector<uint8_t> data(sizeof(float));
    std::memcpy(data.data(), &temperature, sizeof(float));  // Konverze float na bajty
    
    auto [success, response_data] = CanRequestManager::getInstance().sendMessage(can_id, data);
    return success;
}


float ControlModule::getHeaterTemperature() {
    uint32_t can_id = createCanId(Codes::Message_type::Undefined, module, instance, false);
    std::vector<uint8_t> data = {};

    auto [success, response_data] = CanRequestManager::getInstance().sendMessage(can_id, data);

    if (success && response_data.size() >= sizeof(float)) {
        float temperature;
        std::memcpy(&temperature, response_data.data(), sizeof(float));
        return temperature;
    }

    return -1.0f;
}

bool ControlModule::disableHeater() {
    uint32_t can_id = createCanId(Codes::Message_type::Undefined, module, instance, false);
    std::vector<uint8_t> data = {};

    auto [success, response_data] = CanRequestManager::getInstance().sendMessage(can_id, data);
    return success;
}
