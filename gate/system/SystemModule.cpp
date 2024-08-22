#include "system/SystemModule.hpp"
#include "can/CanHandler.hpp"
#include "base/codes.hpp"
#include "can/CanUtils.hpp" 

SystemModule::SystemModule() {}
SystemModule::~SystemModule() {}

oatpp::List<oatpp::Object<MyModuleInfoDto>> SystemModule::getAvailableModules() {
    uint32_t can_id = createCanId(Codes::Message_type::Probe_modules_request, Codes::Module::All, Codes::Instance::Undefined);

    CanHandler canHandler;
    canHandler.sendCanMessage(can_id, nullptr, 0);


    auto modules = oatpp::List<oatpp::Object<MyModuleInfoDto>>::createShared();


    auto coreModule = MyModuleInfoDto::createShared();
    coreModule->module_type = "core";
    coreModule->uid = 0x0123456789abcdef;

    auto controlModule = MyModuleInfoDto::createShared();
    controlModule->module_type = "control";
    controlModule->uid = 0xfedcba9876543210;

    modules->push_back(coreModule);
    modules->push_back(controlModule);

    return modules;
}

oatpp::Object<MyTempDto> SystemModule::getSystemTemperature() {
    uint32_t can_id = createCanId(Codes::Message_type::Probe_modules_response, Codes::Module::Sensor_board, Codes::Instance::Exclusive);

    CanHandler canHandler;
    canHandler.sendCanMessage(can_id, nullptr, 0); 


    auto dto = MyTempDto::createShared();
    dto->temperature = 30.2f;
    return dto;
}
