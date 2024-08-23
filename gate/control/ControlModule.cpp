#include "ControlModule.hpp"

ControlModule::ControlModule() 
    : CommonModule(Codes::Module::Control_board, Codes::Instance::Exclusive) {}

ControlModule& ControlModule::getInstance() {
    static ControlModule instance;
    return instance;
}
