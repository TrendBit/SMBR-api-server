#include "CommonModule.hpp"
#include "can/CanRequestManager.hpp"
#include "can/CanIdGenerator.hpp"  

CommonModule::CommonModule(Codes::Module mod, Codes::Instance inst)
    : module(mod), instance(inst) {}

CommonModule::~CommonModule() {}

float CommonModule::ping() {
    CanRequestManager& canRequestManager = CanRequestManager::getInstance();
    return canRequestManager.ping(createCanId(Codes::Message_type::Ping_request, module, instance, false), 0x00);
}
