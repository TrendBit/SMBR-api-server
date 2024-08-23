#include "CommonModule.hpp"
#include "can/CanRequestManager.hpp"
#include "can/CanIdGenerator.hpp"  

CommonModule::CommonModule(Codes::Module mod, Codes::Instance inst)
    : module(mod), instance(inst) {}

CommonModule::~CommonModule() {}

float CommonModule::ping() {
    uint32_t can_id = createCanId(Codes::Message_type::Ping_request, module, instance, false);
    uint8_t seq_number = 0x00;
    std::vector<uint8_t> data = {seq_number};

    return CanRequestManager::getInstance().sendMessage(can_id, data);
}

