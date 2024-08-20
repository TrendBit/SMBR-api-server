#pragma once
#include "base/CommonModule.hpp"

class SensorModule : public CommonModule {
public:
    SensorModule();
    virtual ~SensorModule();

    float ping() override;  
    float getLoad() override;
    float getCoreTemperature() override;
    bool restart(const std::string& uid) override;
    bool bootloader(const std::string& uid) override;
};
