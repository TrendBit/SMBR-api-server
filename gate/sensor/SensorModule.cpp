#include "SensorModule.hpp"

SensorModule::SensorModule() 
    : CommonModule(Codes::Module::Sensor_board, Codes::Instance::Exclusive) {}

SensorModule::~SensorModule() {}

float SensorModule::ping() {
    return CommonModule::ping(); 
}

float SensorModule::getLoad() {
    return 0.0;
}

float SensorModule::getCoreTemperature() {
    return 0.0;
}

bool SensorModule::restart(const std::string& uid) {
    return false;
}

bool SensorModule::bootloader(const std::string& uid) {
    return false;
}
