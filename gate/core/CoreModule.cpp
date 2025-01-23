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

void CoreModule::getVoltage5V(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback) {
    App_messages::Core::Supply_5V_rail_request getVoltageReq;

    uint32_t requestCanId = createCanId(getVoltageReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t responseCanId = createCanId(App_messages::Core::Supply_5V_rail_response(0).Type(), module, Codes::Instance::Exclusive, false);  

    double timeoutSeconds = 2;

    manager.addRequest(requestCanId, getVoltageReq.Export_data(), responseCanId, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::Core::Supply_5V_rail_response voltageResponse(0);  
            if (voltageResponse.Interpret_data(dataCopy)) {
                callback(voltageResponse.rail_5v);  
            } else {
                callback(-1.0f);  
            }
        } else if (status == CanRequestStatus::Timeout) {
            callback(-2.0f); 
        } else {
            callback(-1.0f);  
        }
    }, timeoutSeconds);
}

void CoreModule::getVoltageVIN(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback) {
    App_messages::Core::Supply_VIN_rail_request getVoltageReq;

    uint32_t requestCanId = createCanId(getVoltageReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t responseCanId = createCanId(App_messages::Core::Supply_VIN_rail_response(0).Type(), module, Codes::Instance::Exclusive, false);  // Passing argument to the constructor

    double timeoutSeconds = 2;

    manager.addRequest(requestCanId, getVoltageReq.Export_data(), responseCanId, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::Core::Supply_VIN_rail_response voltageResponse(0); 
            if (voltageResponse.Interpret_data(dataCopy)) {
                callback(voltageResponse.rail_vin);  
            } else {
                callback(-1.0f);  
            }
        } else if (status == CanRequestStatus::Timeout) {
            callback(-2.0f); 
        } else {
            callback(-1.0f);  
        }
    }, timeoutSeconds);
}

void CoreModule::getVoltagePoE(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback) {
    App_messages::Core::Supply_POE_rail_request getPoEReq;

    uint32_t requestCanId = createCanId(getPoEReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t responseCanId = createCanId(App_messages::Core::Supply_POE_rail_response(0).Type(), module, Codes::Instance::Exclusive, false);

    double timeoutSeconds = 2;

    manager.addRequest(requestCanId, getPoEReq.Export_data(), responseCanId, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::Core::Supply_POE_rail_response poeResponse(0);
            if (poeResponse.Interpret_data(dataCopy)) {
                callback(poeResponse.rail_poe); 
            } else {
                callback(-1.0f); 
            }
        } else if (status == CanRequestStatus::Timeout) {
            callback(-2.0f);  
        } else {
            callback(-1.0f);  
        }
    }, timeoutSeconds);
}

void CoreModule::getCurrentConsumption(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback) {
    App_messages::Core::Supply_current_request getCurrentReq;

    uint32_t requestCanId = createCanId(getCurrentReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t responseCanId = createCanId(App_messages::Core::Supply_current_response(0).Type(), module, Codes::Instance::Exclusive, false);

    double timeoutSeconds = 2;

    manager.addRequest(requestCanId, getCurrentReq.Export_data(), responseCanId, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::Core::Supply_current_response currentResponse(0);
            if (currentResponse.Interpret_data(dataCopy)) {
                callback(currentResponse.current); 
            } else {
                callback(-1.0f); 
            }
        } else if (status == CanRequestStatus::Timeout) {
            callback(-2.0f);  
        } else {
            callback(-1.0f);  
        }
    }, timeoutSeconds);
}

void CoreModule::getPowerDraw(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback) {
    App_messages::Core::Supply_power_draw_request getPowerDrawReq;

    uint32_t requestCanId = createCanId(getPowerDrawReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t responseCanId = createCanId(App_messages::Core::Supply_power_draw_response(0).Type(), module, Codes::Instance::Exclusive, false);

    double timeoutSeconds = 2;

    manager.addRequest(requestCanId, getPowerDrawReq.Export_data(), responseCanId, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::Core::Supply_power_draw_response powerDrawResponse(0);
            if (powerDrawResponse.Interpret_data(dataCopy)) {
                callback(powerDrawResponse.power_draw); 
            } else {
                callback(-1.0f); 
            }
        } else if (status == CanRequestStatus::Timeout) {
            callback(-2.0f);  
        } else {
            callback(-1.0f);  
        }
    }, timeoutSeconds);
}








