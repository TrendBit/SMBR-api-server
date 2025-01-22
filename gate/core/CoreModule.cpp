#include "CoreModule.hpp"

CoreModule::CoreModule(boost::asio::io_context& io_context, CanRequestManager& canRequestManager)
    : m_ioContext(io_context), m_canRequestManager(canRequestManager) {}

void CoreModule::getShortID(CanRequestManager& manager, Codes::Module module, std::function<void(std::string)> callback) {
    App_messages::Core::SID_request getSIDReq;

    uint32_t requestCanId = createCanId(getSIDReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t responseCanId = createCanId(App_messages::Core::SID_response().Type(), module, Codes::Instance::Exclusive, false);

    double timeoutSeconds = 2;

    manager.addRequest(requestCanId, getSIDReq.Export_data(), responseCanId, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::Core::SID_response sidResponse;
            if (sidResponse.Interpret_data(dataCopy)) {
                std::ostringstream sidStream;
                sidStream << std::hex << std::setw(4) << std::setfill('0') << sidResponse.sid;  
                callback(sidStream.str());  
            } else {
                callback(""); 
            }
        } else if (status == CanRequestStatus::Timeout) {
            callback("timeout");  
        } else {
            callback("");  
        }
    }, timeoutSeconds);
}

void CoreModule::getIpAddress(CanRequestManager& manager, Codes::Module module, std::function<void(std::string)> callback) {
    App_messages::Core::IP_address_request getIpRequest;

    uint32_t requestCanId = createCanId(getIpRequest.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t responseCanId = createCanId(App_messages::Core::IP_address_response().Type(), module, Codes::Instance::Exclusive, false);

    double timeoutSeconds = 2;

    manager.addRequest(requestCanId, getIpRequest.Export_data(), responseCanId, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::Core::IP_address_response ipResponse;
            if (ipResponse.Interpret_data(dataCopy)) {
                if (ipResponse.IP_address.empty()) {
                    callback("");  
                } else {
                    std::string ipString = std::to_string(ipResponse.IP_address[0]) + "." +
                                           std::to_string(ipResponse.IP_address[1]) + "." +
                                           std::to_string(ipResponse.IP_address[2]) + "." +
                                           std::to_string(ipResponse.IP_address[3]);
                    callback(ipString);  
                }
            } else {
                callback(""); 
            }
        } else if (status == CanRequestStatus::Timeout) {
            callback("timeout");  
        } else {
            callback("");  
        }
    }, timeoutSeconds);
}



void CoreModule::getHostname(CanRequestManager& manager, Codes::Module module, std::function<void(std::string)> callback) {
    App_messages::Core::Hostname_request getHostnameReq;

    uint32_t requestCanId = createCanId(getHostnameReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t responseCanId = createCanId(App_messages::Core::Hostname_response().Type(), module, Codes::Instance::Exclusive, false);

    double timeoutSeconds = 2;

    manager.addRequest(requestCanId, getHostnameReq.Export_data(), responseCanId, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::Core::Hostname_response hostnameResponse;
            if (hostnameResponse.Interpret_data(dataCopy)) {
                std::string hostname = hostnameResponse.hostname.substr(0, 8);  
                callback(hostname);
            } else {
                callback("");
            }
        } else if (status == CanRequestStatus::Timeout) {
            callback("timeout");
        } else {
            callback("");
        }
    }, timeoutSeconds);
}

void CoreModule::getSerialNumber(CanRequestManager& manager, Codes::Module module, std::function<void(int64_t)> callback) {
    App_messages::Core::Serial_request getSerialReq;

    uint32_t requestCanId = createCanId(getSerialReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t responseCanId = createCanId(App_messages::Core::Serial_response().Type(), module, Codes::Instance::Exclusive, false);

    double timeoutSeconds = 2;

    manager.addRequest(requestCanId, getSerialReq.Export_data(), responseCanId, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::Core::Serial_response serialResponse;
            if (serialResponse.Interpret_data(dataCopy)) {
                callback(serialResponse.serial_number);  
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

void CoreModule::getPowerSupplyType(CanRequestManager& manager, Codes::Module module, std::function<void(bool, bool, bool, bool)> callback) {
    App_messages::Core::Supply_type_request getSupplyTypeReq;

    uint32_t requestCanId = createCanId(getSupplyTypeReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t responseCanId = createCanId(App_messages::Core::Supply_type_response().Type(), module, Codes::Instance::Exclusive, false);

    double timeoutSeconds = 2;

    manager.addRequest(requestCanId, getSupplyTypeReq.Export_data(), responseCanId, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::Core::Supply_type_response supplyTypeResponse;
            if (supplyTypeResponse.Interpret_data(dataCopy)) {
                callback(true, 
                        supplyTypeResponse.vin, 
                        supplyTypeResponse.poe, 
                        supplyTypeResponse.poe_hb); 
            } else {
                callback(false, false, false, false); 
            }
        } else if (status == CanRequestStatus::Timeout) {
            callback(false, false, false, false);
        } else {
            callback(false, false, false, false); 
        }
    }, timeoutSeconds);

}








