#pragma once
#include "can/CanHandler.hpp"
#include <vector>
#include <string>

class SystemModule {
public:
    SystemModule();
    virtual ~SystemModule();

    std::vector<std::string> getAvailableModules();
    float getSystemTemperature();

private:
    CanHandler canHandler;
};