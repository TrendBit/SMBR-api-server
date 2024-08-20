#pragma once
#include "base/CommonModule.hpp"

class CoreModule : public CommonModule {
public:
    CoreModule();
    virtual ~CoreModule();

    float ping() override;  
    float getLoad() override;
    float getCoreTemperature() override;
    bool restart(const std::string& uid) override;
    bool bootloader(const std::string& uid) override;
};
