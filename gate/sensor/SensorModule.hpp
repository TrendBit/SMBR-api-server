#pragma once
#include "base/CommonModule.hpp"
#include "../../oatpp-server/dto/MyTempDto.hpp"

class SensorModule : public CommonModule {
public:
    SensorModule();
    virtual ~SensorModule();

    float ping() override;  
    /*float getLoad() override;
    float getCoreTemperature() override;
    bool restart(const std::string& uid) override;
    bool bootloader(const std::string& uid) override;*/

    oatpp::Object<MyTempDto> getTopSensorTemperature();
    oatpp::Object<MyTempDto> getBottomSensorTemperature();
};
