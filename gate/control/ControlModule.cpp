#include "ControlModule.hpp"

ControlModule::ControlModule() 
    : CommonModule(Codes::Module::Control_board, Codes::Instance::Exclusive) {}

ControlModule& ControlModule::getInstance() {
    static ControlModule instance;
    return instance;
}

void ControlModule::setHeaterTemperature(boost::asio::io_context& io_context, float temperature, std::function<void(bool)> handler) {
    uint32_t can_id = createCanId(Codes::Message_type::Undefined, module, instance, false);
    std::vector<uint8_t> data(sizeof(float));
    std::memcpy(data.data(), &temperature, sizeof(float)); 

    CanRequestManager::getInstance(io_context).addRequest(can_id, data, 
        [handler](bool success, const std::vector<uint8_t>&) {
            handler(success);
        }
    );
}

void ControlModule::getHeaterTemperature(boost::asio::io_context& io_context, std::function<void(float)> handler) {
    uint32_t can_id = createCanId(Codes::Message_type::Undefined, module, instance, false);
    std::vector<uint8_t> data = {};

    CanRequestManager::getInstance(io_context).addRequest(can_id, data, 
        [handler](bool success, const std::vector<uint8_t>& response_data) {
            if (success && response_data.size() >= sizeof(float)) {
                float temperature;
                std::memcpy(&temperature, response_data.data(), sizeof(float));
                handler(temperature);
            } else {
                handler(-1.0f);
            }
        }
    );
}

void ControlModule::disableHeater(boost::asio::io_context& io_context, std::function<void(bool)> handler) {
    uint32_t can_id = createCanId(Codes::Message_type::Undefined, module, instance, false);
    std::vector<uint8_t> data = {};

    CanRequestManager::getInstance(io_context).addRequest(can_id, data, 
        [handler](bool success, const std::vector<uint8_t>&) {
            handler(success);
        }
    );
}
