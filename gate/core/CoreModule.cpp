#include "CoreModule.hpp"

CoreModule::CoreModule() 
    : CommonModule(Codes::Module::Core_device, Codes::Instance::Exclusive) {}

CoreModule& CoreModule::getInstance() {
    static CoreModule instance;
    return instance;
}
