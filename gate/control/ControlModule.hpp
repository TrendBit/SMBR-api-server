#pragma once

#include "base/CommonModule.hpp"

class ControlModule : public CommonModule {
public:
    static ControlModule& getInstance(); 

private:
    ControlModule();  
    ControlModule(const ControlModule&) = delete;
    ControlModule& operator=(const ControlModule&) = delete;
    ~ControlModule() = default;
};

