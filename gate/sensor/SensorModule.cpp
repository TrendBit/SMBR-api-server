#include "SensorModule.hpp"

SensorModule::SensorModule() 
    : CommonModule(Codes::Module::Sensor_board, Codes::Instance::Exclusive) {}

SensorModule& SensorModule::getInstance() {
    static SensorModule instance;
    return instance;
}

void SensorModule::getTopTemperature(boost::asio::io_context& io_context, std::function<void(float)> handler) {
    uint32_t can_id = createCanId(Codes::Message_type::Undefined, module, instance, false);
    std::vector<uint8_t> data = {};

    CanRequestManager::getInstance(io_context).addRequest(can_id, data, [handler](bool success, const std::vector<uint8_t>& response_data) {
        if (success && response_data.size() >= sizeof(float)) {
            float temperature;
            std::memcpy(&temperature, response_data.data(), sizeof(float));
            handler(temperature);
        } else {
            handler(-1.0f);
        }
    });
}

void SensorModule::getBottomTemperature(boost::asio::io_context& io_context, std::function<void(float)> handler) {
    uint32_t can_id = createCanId(Codes::Message_type::Undefined, module, instance, false);
    std::vector<uint8_t> data = {};

    CanRequestManager::getInstance(io_context).addRequest(can_id, data, [handler](bool success, const std::vector<uint8_t>& response_data) {
        if (success && response_data.size() >= sizeof(float)) {
            float temperature;
            std::memcpy(&temperature, response_data.data(), sizeof(float));
            handler(temperature);
        } else {
            handler(-1.0f);
        }
    });
}
