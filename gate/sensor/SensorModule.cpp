#include "SensorModule.hpp"

SensorModule::SensorModule() 
    : CommonModule(Codes::Module::Sensor_board, Codes::Instance::Exclusive) {}

SensorModule& SensorModule::getInstance() {
    static SensorModule instance;
    return instance;
}
