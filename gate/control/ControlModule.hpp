#pragma once
#include "base/CommonModule.hpp"
#include "../../oatpp-server/dto/MyTempDto.hpp"
#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"

class ControlModule : public CommonModule {
private:
    float currentTemperature = 0.0f; 
    bool heaterEnabled = true;         
public:
    ControlModule();
    virtual ~ControlModule();

    float ping() override;
    /*float getLoad() override;
    float getCoreTemperature() override;
    bool restart(const std::string& uid) override;
    bool bootloader(const std::string& uid) override;
    */

    bool setHeaterTemperature(float temperature);
    oatpp::Object<MyTempDto> getHeaterTemperature();
    bool disableHeater();
};
