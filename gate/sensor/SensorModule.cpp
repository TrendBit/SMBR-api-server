#include "SensorModule.hpp"


SensorModule::SensorModule() 
    : CommonModule(Codes::Module::Sensor_board, Codes::Instance::Exclusive) {}

SensorModule& SensorModule::getInstance() {
    static SensorModule instance;
    return instance;
}

std::future<float> SensorModule::getTopTemperature() {
    return std::async(std::launch::async, [this]() {
        uint32_t can_id = createCanId(Codes::Message_type::Undefined, module, instance, false);  // Upravit Message_type na správnou hodnotu
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

std::future<float> SensorModule::getBottomTemperature() {
    return std::async(std::launch::async, [this]() {
        uint32_t can_id = createCanId(Codes::Message_type::Undefined, module, instance, false);  // Upravit Message_type na správnou hodnotu
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
