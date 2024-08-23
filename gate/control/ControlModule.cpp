#include "ControlModule.hpp"

ControlModule::ControlModule() 
    : CommonModule(Codes::Module::Control_board, Codes::Instance::Exclusive) {}

ControlModule::~ControlModule() {}
