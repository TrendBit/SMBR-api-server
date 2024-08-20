#include "SystemModule.hpp"

SystemModule::SystemModule() {}
SystemModule::~SystemModule() {}

std::vector<std::string> SystemModule::getAvailableModules() {
    uint32_t can_id = 0x300; 
    return canHandler.sendAvailableModulesRequest(can_id);
}

float SystemModule::getSystemTemperature() {
    uint32_t can_id = 0x301;  
    return canHandler.sendSystemTemperatureRequest(can_id);
}