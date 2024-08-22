#include "ControlModule.hpp"

ControlModule::ControlModule() 
    : CommonModule(Codes::Module::Control_board, Codes::Instance::Exclusive) {}

ControlModule::~ControlModule() {}

float ControlModule::ping() {
    return CommonModule::ping(); 
}
/*
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
}*/

bool ControlModule::setHeaterTemperature(float temperature) {
    currentTemperature = temperature;
    return true;
}

oatpp::Object<MyTempDto> ControlModule::getHeaterTemperature() {
    auto dto = MyTempDto::createShared();
    dto->temperature = currentTemperature;
    return dto;
}

bool ControlModule::disableHeater() { 
    bool heaterEnabled = false;
    return true;
}
