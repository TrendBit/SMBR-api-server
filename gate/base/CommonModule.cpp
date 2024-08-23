#include "CommonModule.hpp"
#include "can/CanService.hpp"
#include "can/CanUtils.hpp"

CommonModule::CommonModule(Codes::Module mod, Codes::Instance inst)
    : module(mod), instance(inst) {}

CommonModule::~CommonModule() {}

float CommonModule::ping() {
    CanService canService;
    return canService.ping(createCanId(Codes::Message_type::Ping_request, module, instance, false), 0x00);
}


