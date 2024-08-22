#include "core/CoreModule.hpp"

CoreModule::CoreModule()
    : CommonModule(Codes::Module::Core_device, Codes::Instance::Exclusive) {}

CoreModule::~CoreModule() {}

float CoreModule::ping() {
    return CommonModule::ping(); 
}

/*float CoreModule::getLoad() {
    return 0.0;
}

float CoreModule::getCoreTemperature() {
    return 0.0;
}

bool CoreModule::restart(const std::string& uid) {
    return false;
}

bool CoreModule::bootloader(const std::string& uid) {
    return false;
}*/

oatpp::Object<MySupplyTypeResponseDto> CoreModule::getSupplyType() {
    uint32_t can_id = createCanId(Codes::Message_type::Supply_type_request, Codes::Module::Core_device, Codes::Instance::Instance_6);

    CanHandler canHandler;
    bool success = canHandler.sendSupplyTypeRequest(can_id);

    auto dto = MySupplyTypeResponseDto::createShared();
    
    if (success) {

        dto->adapter = true;  
        dto->poe = false;
    } else {
        dto->adapter = false;
        dto->poe = false;
    }

    return dto;
}
