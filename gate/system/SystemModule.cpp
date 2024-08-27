#include "SystemModule.hpp"

SystemModule& SystemModule::getInstance() {
    static SystemModule instance;
    return instance;
}

std::optional<std::vector<uint8_t>> SystemModule::getAvailableModules() {
    uint32_t can_id = createCanId(Codes::Message_type::Probe_modules_request, Codes::Module::All, Codes::Instance::Undefined, false);
    std::vector<uint8_t> data = {};  

    auto [success, response_data] = CanRequestManager::getInstance().sendMessage(can_id, data);

    if (success && !response_data.empty()) {
        return response_data;
    }

    return std::nullopt;
}

float SystemModule::getSystemTemperature() {
    // TODO - change Message_type by codes.hpp
    uint32_t can_id = createCanId(Codes::Message_type::Undefined, Codes::Module::Sensor_board, Codes::Instance::Exclusive, false);
    std::vector<uint8_t> data = {};  

    auto [success, response_data] = CanRequestManager::getInstance().sendMessage(can_id, data);

    // TODO - real data processing
    if (success) { 
        float temp = 40.0f;
        return temp;
    }

    return -1.0f; 
}
