#include "SystemModule.hpp"

SystemModule& SystemModule::getInstance() {
    static SystemModule instance;
    return instance;
}

std::future<std::optional<std::vector<uint8_t>>> SystemModule::getAvailableModules() {
    return std::async(std::launch::async, [this]() {
        uint32_t can_id = createCanId(Codes::Message_type::Probe_modules_request, Codes::Module::All, Codes::Instance::Undefined, false);
        std::vector<uint8_t> data = {};  

        auto [success, response_data] = CanRequestManager::getInstance().sendMessageAsync(can_id, data).get();

        if (success && !response_data.empty()) {
            return std::make_optional(response_data);
        }

        return std::optional<std::vector<uint8_t>>{};
    });
}

std::future<float> SystemModule::getSystemTemperature() {
    return std::async(std::launch::async, [this]() {
        uint32_t can_id = createCanId(Codes::Message_type::Undefined, Codes::Module::Sensor_board, Codes::Instance::Exclusive, false);
        std::vector<uint8_t> data = {};  

        auto [success, response_data] = CanRequestManager::getInstance().sendMessageAsync(can_id, data).get();

        if (success) { 
            float temp = 40.0f;
            return temp;
        }

        return -1.0f;
    });
}
