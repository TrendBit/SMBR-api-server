#include "ControlModule.hpp"

ControlModule::ControlModule() 
    : CommonModule(Codes::Module::Control_board, Codes::Instance::Exclusive) {}

ControlModule::~ControlModule() {}

float ControlModule::getLoad() {
    return 0.0;
}

float ControlModule::getCoreTemperature() {
    return 0.0;
}

bool ControlModule::restart(const std::string& uid) {
    return false;
}

bool ControlModule::bootloader(const std::string& uid) {
    return false;
}