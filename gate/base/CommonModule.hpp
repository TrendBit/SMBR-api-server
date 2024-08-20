#pragma once
#include "can/CanHandler.hpp"
#include "base/codes.hpp"
#include <string>

class CommonModule {
public:
    CommonModule(Codes::Module module, Codes::Instance instance);
    virtual ~CommonModule();

    virtual float ping();                        
    virtual float getLoad();
    virtual float getCoreTemperature();
    virtual bool restart(const std::string& uid);
    virtual bool bootloader(const std::string& uid);

protected:
    CanHandler canHandler;
    Codes::Module module;
    Codes::Instance instance;

    uint32_t createCanId(Codes::Message_type messageType, bool emergencyFlag = false);

private:
    uint32_t extractCanIdFromResponse(const std::string& response);
};
