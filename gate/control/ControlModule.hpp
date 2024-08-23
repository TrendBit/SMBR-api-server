#ifndef CONTROLMODULE_HPP
#define CONTROLMODULE_HPP

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

#endif // CONTROLMODULE_HPP
