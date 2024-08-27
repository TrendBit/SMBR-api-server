#include "CoreModule.hpp"
#include "can/CanRequestManager.hpp"
#include "can/CanIdGenerator.hpp"

CoreModule::CoreModule()
    : CommonModule(Codes::Module::Core_device, Codes::Instance::Exclusive) {}

CoreModule& CoreModule::getInstance() {
    static CoreModule instance;
    return instance;
}

bool CoreModule::getSupplyType(bool& adapter, bool& poe) {
    uint32_t can_id = createCanId(Codes::Message_type::Supply_type_request, module, instance, false);
    std::vector<uint8_t> data = {}; 

    auto [success, response_data] = CanRequestManager::getInstance().sendMessage(can_id, data);

    if (success && response_data.size() >= 2) {
        adapter = response_data[0] == 0x01;
        poe = response_data[1] == 0x01;
        return true;
    }

    return false;
}
