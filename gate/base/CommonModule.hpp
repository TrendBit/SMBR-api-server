#pragma once
#include "can/CanHandler.hpp"
#include "codes/codes.hpp"
#include <string>

class CommonModule {
public:
    CommonModule(Codes::Module module, Codes::Instance instance);
    virtual ~CommonModule();

    virtual float ping();                        
    virtual float getLoad();
    virtual float getCoreTemperature();
    virtual int getCoreCount();
    virtual bool restart(const std::string& uid);
    virtual bool bootloader(const std::string& uid);

protected:
    CanHandler canHandler;
    Codes::Module module;
    Codes::Instance instance;


};
