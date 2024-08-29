#include "CoreModule.hpp"

CoreModule::CoreModule()
    : CommonModule(Codes::Module::Core_device, Codes::Instance::Exclusive) {}

CoreModule& CoreModule::getInstance() {
    static CoreModule instance;
    return instance;
}

void CoreModule::getSupplyType(boost::asio::io_context& io_context, std::function<void(bool, bool, bool)> handler) {
    uint32_t can_id = createCanId(Codes::Message_type::Supply_type_request, module, instance, false);
    std::vector<uint8_t> data = {};

    CanRequestManager::getInstance(io_context).addRequest(can_id, data, [handler](bool success, const std::vector<uint8_t>& response_data) {
        if (success && response_data.size() >= 2) {
            bool adapter = response_data[0] == 0x01;
            bool poe = response_data[1] == 0x01;
            handler(true, adapter, poe);
        } else {
            handler(false, false, false);
        }
    });
}
