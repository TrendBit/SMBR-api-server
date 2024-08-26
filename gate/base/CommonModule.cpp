#include "CommonModule.hpp"
#include "can/CanRequestManager.hpp"
#include "can/CanIdGenerator.hpp"
#include <chrono>


CommonModule::CommonModule(Codes::Module mod, Codes::Instance inst)
    : module(mod), instance(inst) {}

CommonModule::~CommonModule() {}

float CommonModule::ping() {
    uint32_t can_id = createCanId(Codes::Message_type::Ping_request, module, instance, false);
    uint8_t seq_number = 0x00;
    std::vector<uint8_t> data = {seq_number};

    auto start_time = std::chrono::steady_clock::now();

    auto [success, response_data] = CanRequestManager::getInstance().sendMessage(can_id, data);

    auto end_time = std::chrono::steady_clock::now();

    if (success) {
        float elapsed_time = std::chrono::duration<float, std::milli>(end_time - start_time).count();
        return elapsed_time;
    } else {
        return -1.0f;
    }
}

bool CommonModule::getLoad(float& load, int& cores) {
    uint32_t can_id = createCanId(Codes::Message_type::Core_load_request, module, instance, false);
    std::vector<uint8_t> data = {}; 

    auto [success, response_data] = CanRequestManager::getInstance().sendMessage(can_id, data);

    // TODO - real data processing
    if (success) {
        load = 20.0f; 
        cores = 4;    

        return true;
    } else {
        return false;
    }
}

bool CommonModule::getCoreTemp(float& core_temp) {
    uint32_t can_id = createCanId(Codes::Message_type::Core_temperature_request, module, instance, false);
    std::vector<uint8_t> data = {}; 

    auto [success, response_data] = CanRequestManager::getInstance().sendMessage(can_id, data);

    // TODO - real data processing
    if (success) {
        core_temp = 35.0f;  
        return true;
    } else {
        return false;
    }
}

bool CommonModule::restart(const std::string& uid) {
    uint32_t can_id = createCanId(Codes::Message_type::Device_reset, module, instance, false);
    std::vector<uint8_t> data(uid.begin(), uid.end());

    auto [success, response_data] = CanRequestManager::getInstance().sendMessage(can_id, data);

    return success;
}

bool CommonModule::bootloader(const std::string& uid) {
    uint32_t can_id = createCanId(Codes::Message_type::Device_bootloader, module, instance, false);
    std::vector<uint8_t> data(uid.begin(), uid.end());

    auto [success, response_data] = CanRequestManager::getInstance().sendMessage(can_id, data);

    return success;
}


