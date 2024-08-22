#ifndef SystemModule_hpp
#define SystemModule_hpp

#include "base/CommonModule.hpp" 
#include <vector>
#include <string>
#include "../../oatpp-server/dto/MyModuleInfoDto.hpp"
#include "../../oatpp-server/dto/MyTempDto.hpp"
#include "can/CanHandler.hpp"

class SystemModule {
public:
    SystemModule();
    ~SystemModule();

    oatpp::List<oatpp::Object<MyModuleInfoDto>> getAvailableModules();
    oatpp::Object<MyTempDto> getSystemTemperature();

private:
    CanHandler canHandler;
};

#endif /* SystemModule_hpp */
