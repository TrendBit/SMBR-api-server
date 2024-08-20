#include "core/CoreModule.hpp"

CoreModule::CoreModule()
    : CommonModule(Codes::Module::Core_device, Codes::Instance::Exclusive) {}

CoreModule::~CoreModule() {}

float CoreModule::ping() {
    return CommonModule::ping(); 
}

float CoreModule::getLoad() {
    return 0.0;
}

float CoreModule::getCoreTemperature() {
    return 0.0;
}

bool CoreModule::restart(const std::string& uid) {
    return false;
}

bool CoreModule::bootloader(const std::string& uid) {
    return false;
}
