/*#pragma once
#include "base/CommonModule.hpp"
#include "../../oatpp-server/dto/MySupplyTypeResponseDto.hpp"
#include "oatpp/macro/codegen.hpp" 
#include "oatpp/Types.hpp" 
#include "can/CanUtils.hpp"


class CoreModule : public CommonModule {
public:
    CoreModule();
    virtual ~CoreModule();

    oatpp::Object<MySupplyTypeResponseDto> getSupplyType();

    float ping() override;  
    /*float getLoad() override;
    float getCoreTemperature() override;
    bool restart(const std::string& uid) override;
    bool bootloader(const std::string& uid) override;
    
};
*/