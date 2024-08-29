#include "SystemModule.hpp"

SystemModule& SystemModule::getInstance() {
    static SystemModule instance;
    return instance;
}

void SystemModule::getAvailableModules(boost::asio::io_context& io_context, std::function<void(std::optional<std::vector<uint8_t>>)> handler) {
    uint32_t can_id = createCanId(Codes::Message_type::Probe_modules_request, Codes::Module::All, Codes::Instance::Undefined, false);
    std::vector<uint8_t> data = {};  

    CanRequestManager::getInstance(io_context).addRequest(can_id, data, [handler](bool success, const std::vector<uint8_t>& response_data) {
        if (success && !response_data.empty()) {
            handler(std::make_optional(response_data));
        } else {
            handler(std::nullopt);
        }
    });
}

void SystemModule::getSystemTemperature(boost::asio::io_context& io_context, std::function<void(float)> handler) {
    uint32_t can_id = createCanId(Codes::Message_type::Undefined, Codes::Module::Sensor_board, Codes::Instance::Exclusive, false);
    std::vector<uint8_t> data = {};  

    CanRequestManager::getInstance(io_context).addRequest(can_id, data, [handler](bool success, const std::vector<uint8_t>&) {
        if (success) { 
            float temp = 40.0f; 
            handler(temp);
        } else {
            handler(-1.0f);
        }
    });
}
