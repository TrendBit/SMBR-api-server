#include "CodesUtils.hpp"

std::string getInstanceString(Codes::Instance instance) {
    switch (instance) {
        case Codes::Instance::Undefined: return "Undefined";
        case Codes::Instance::Exclusive: return "Exclusive";
        case Codes::Instance::All: return "All";
        case Codes::Instance::Reserved: return "Reserved";
        case Codes::Instance::Instance_1: return "Instance_1";
        case Codes::Instance::Instance_2: return "Instance_2";
        case Codes::Instance::Instance_3: return "Instance_3";
        case Codes::Instance::Instance_4: return "Instance_4";
        case Codes::Instance::Instance_5: return "Instance_5";
        case Codes::Instance::Instance_6: return "Instance_6";
        case Codes::Instance::Instance_7: return "Instance_7";
        case Codes::Instance::Instance_8: return "Instance_8";
        case Codes::Instance::Instance_9: return "Instance_9";
        case Codes::Instance::Instance_10: return "Instance_10";
        case Codes::Instance::Instance_11: return "Instance_11";
        case Codes::Instance::Instance_12: return "Instance_12";
        default: return "Unknown";
    }
}

std::string getModuleString(Codes::Module module) {
    switch (module) {
        case Codes::Module::Core_module: return "core";
        case Codes::Module::Control_module: return "control";
        case Codes::Module::Sensor_module: return "sensor";
        default: return "unknown";
    }
}
