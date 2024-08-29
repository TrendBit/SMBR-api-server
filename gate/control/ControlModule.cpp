#include "ControlModule.hpp"



ControlModule::ControlModule() 
    : CommonModule(Codes::Module::Control_board, Codes::Instance::Exclusive) {}

ControlModule& ControlModule::getInstance() {
    static ControlModule instance;
    return instance;
}

std::future<bool> ControlModule::setHeaterTemperature(float temperature) {
    return std::async(std::launch::async, [this, temperature]() {
        uint32_t can_id = createCanId(Codes::Message_type::Undefined, module, instance, false);
        std::vector<uint8_t> data(sizeof(float));
        std::memcpy(data.data(), &temperature, sizeof(float)); 

        auto [success, response_data] = CanRequestManager::getInstance().sendMessageAsync(can_id, data).get();
        return success;
    });
}

std::future<float> ControlModule::getHeaterTemperature() {
    return std::async(std::launch::async, [this]() {
        uint32_t can_id = createCanId(Codes::Message_type::Undefined, module, instance, false);
        std::vector<uint8_t> data = {};

        auto [success, response_data] = CanRequestManager::getInstance().sendMessageAsync(can_id, data).get();

        if (success && response_data.size() >= sizeof(float)) {
            float temperature;
            std::memcpy(&temperature, response_data.data(), sizeof(float));
            return temperature;
        }

        return -1.0f;
    });
}

std::future<bool> ControlModule::disableHeater() {
    return std::async(std::launch::async, [this]() {
        uint32_t can_id = createCanId(Codes::Message_type::Undefined, module, instance, false);
        std::vector<uint8_t> data = {};

        auto [success, response_data] = CanRequestManager::getInstance().sendMessageAsync(can_id, data).get();
        return success;
    });
}
