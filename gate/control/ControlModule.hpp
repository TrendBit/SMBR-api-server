#pragma once
#include "base/CommonModule.hpp"

class ControlModule : public CommonModule {
public:
    ControlModule();
    virtual ~ControlModule();

    float getLoad() override;
    float getCoreTemperature() override;
    bool restart(const std::string& uid) override;
    bool bootloader(const std::string& uid) override;
};