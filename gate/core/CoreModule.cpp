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
                std::string ipString;
                if (ipResponse.IP_address.empty()) {
                    ipString = "";  
                } else {
                    ipString = std::to_string(ipResponse.IP_address[0]) + "." +
                               std::to_string(ipResponse.IP_address[1]) + "." +
                               std::to_string(ipResponse.IP_address[2]) + "." +
                               std::to_string(ipResponse.IP_address[3]);
                }
                callback(ipString);
            } else {
                callback("");  
            }
        } else if (status == CanRequestStatus::Timeout) {
            callback("");  
        } else {
            callback("");  
        }
    }, timeoutSeconds);
}





