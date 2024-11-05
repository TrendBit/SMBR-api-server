#include "CommonModule.hpp"
#include <iostream>
#include <iomanip> 
#include <chrono>



CommonModule::CommonModule(boost::asio::io_context& io_context, CanRequestManager& canRequestManager)
    : m_ioContext(io_context), m_canRequestManager(canRequestManager) {}


void CommonModule::ping(CanRequestManager& manager, Codes::Module module, uint8_t seq_num, std::function<void(float)> callback) {
    
    App_messages::Ping_request set_pingReq((uint8_t)seq_num);
    App_messages::Ping_response set_pingRes;
    
    uint32_t ping_can_id = createCanId(set_pingReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t ping_response_id = createCanId(set_pingRes.Type(), module, Codes::Instance::Exclusive, false);
     
    double timeoutSeconds = 1;

    auto start_time = std::chrono::steady_clock::now();

    manager.addRequestWithSeq(ping_can_id, set_pingReq.Export_data(), ping_response_id, seq_num, [callback, start_time](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            auto end_time = std::chrono::steady_clock::now();
            float response_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
            callback(response_time_ms); 
        } else if (status == CanRequestStatus::Timeout) {
            callback(-2); 
        } else {
            callback(-1); 
        }
    }, timeoutSeconds);
}


void CommonModule::getCoreLoad(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback) {
    /*App_messages::Core_load_request loadRequest;

    uint32_t load_can_id = createCanId(loadRequest.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t load_response_id = createCanId(App_messages::Core_load_response().Type(), module, Codes::Instance::Exclusive, false);*/
//----------------------------------------
    uint32_t load_can_id = createCanId(Codes::Message_type::Core_load_request, module, Codes::Instance::Exclusive, false);
    uint32_t load_response_id = createCanId(Codes::Message_type::Core_load_response, module, Codes::Instance::Exclusive, false);
    std::vector<uint8_t> load_data = {};  
//----------------------------------------
    double timeoutSeconds = 2;

    manager.addRequest(load_can_id, /*loadRequest.Export_data()*/load_data, load_response_id, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            //App_messages::Core_load_response loadResponse;
            App_messages::Core_temp_response loadResponse;
            if (loadResponse.Interpret_data(dataCopy)) {
                //callback(loadResponse.load);
                callback(loadResponse.temperature);
            } else {
                callback(-1);
            }
        } else if (status == CanRequestStatus::Timeout) {
            callback(-2);
        } else {
            callback(-1);
        }
    }, timeoutSeconds);
}


void CommonModule::getCoreTemp(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback) {
    App_messages::Core_temp_request set_coreTempReq;

    uint32_t temp_can_id = createCanId(set_coreTempReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t temp_response_id = createCanId(App_messages::Core_temp_response().Type(), module, Codes::Instance::Exclusive, false);

    double timeoutSeconds = 2;

    manager.addRequest(temp_can_id, set_coreTempReq.Export_data(), temp_response_id, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::Core_temp_response coreTempRes;
            if (coreTempRes.Interpret_data(dataCopy)) {
                callback(coreTempRes.temperature);
            } else {
                callback(-1);  
            }
        } else if (status == CanRequestStatus::Timeout) {
            callback(-2);  
        } else {
            callback(-1);  
        }
    }, timeoutSeconds);
}

void CommonModule::sendDeviceReset(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback) {
    App_messages::Device_reset resetRequest; 
        
    uint32_t reset_can_id = createCanId(resetRequest.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t reset_response_can_id = createCanId(resetRequest.Type(), module, Codes::Instance::Exclusive, false);

    double timeoutSeconds = 0;  

    manager.addRequest(reset_can_id, resetRequest.Export_data(), reset_response_can_id, [callback](CanRequestStatus status, const CanMessage& response) {
        callback(true);
    }, timeoutSeconds);
}

void CommonModule::sendDeviceUsbBootloader(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback) {
    App_messages::Device_usb_bootloader usbBootloaderRequest;
        
    uint32_t usbBootloader_can_id = createCanId(usbBootloaderRequest.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t usbBootloader_response_can_id = createCanId(usbBootloaderRequest.Type(), module, Codes::Instance::Exclusive, false);

    int timeoutSeconds = 0;  

    manager.addRequest(usbBootloader_can_id, usbBootloaderRequest.Export_data(), usbBootloader_response_can_id, [callback](CanRequestStatus status, const CanMessage& response) {
        callback(true);
    }, timeoutSeconds);
}

void CommonModule::sendDeviceCanBootloader(CanRequestManager& manager, Codes::Module module, std::function<void(bool)> callback) {
    App_messages::Device_can_bootloader canBootloaderRequest;
        
    uint32_t canBootloader_can_id = createCanId(canBootloaderRequest.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t canBootloader_response_can_id = createCanId(canBootloaderRequest.Type(), module, Codes::Instance::Exclusive, false);

    int timeoutSeconds = 0;  

    manager.addRequest(canBootloader_can_id, canBootloaderRequest.Export_data(), canBootloader_response_can_id, [callback](CanRequestStatus status, const CanMessage& response) {
        callback(status == CanRequestStatus::Success);
    }, timeoutSeconds);
}






