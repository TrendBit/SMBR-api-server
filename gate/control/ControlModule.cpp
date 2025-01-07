#include "ControlModule.hpp"



ControlModule::ControlModule(boost::asio::io_context& io_context, CanRequestManager& canRequestManager)
    : m_ioContext(io_context), m_canRequestManager(canRequestManager) {}


void ControlModule::setIntensity(Codes::Module module, float intensity, int channel, std::function<void(bool)> callback) {

    App_messages::LED_panel::Set_intensity Set_intensity((uint8_t)channel, (float)intensity);

    uint32_t intensity_can_id = createCanId(Set_intensity.Type(), module, Codes::Instance::Exclusive, false);

    m_canRequestManager.sendWithoutResponse(intensity_can_id, Set_intensity.Export_data(), [callback](bool success) {
        callback(success);
    });
}

void ControlModule::getIntensity(CanRequestManager& manager, Codes::Module module, int channel, std::function<void(float)> callback) {
    App_messages::LED_panel::Get_intensity_request getIntensityReq ((uint8_t)channel);

    uint32_t requestCanId = createCanId(getIntensityReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t responseCanId = createCanId(App_messages::LED_panel::Get_intensity_response().Type(), module, Codes::Instance::Exclusive, false);

    double timeoutSeconds = 2;

    manager.addRequest(requestCanId, getIntensityReq.Export_data(), responseCanId, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::LED_panel::Get_intensity_response intensityResponse;
            if (intensityResponse.Interpret_data(dataCopy)) {
                callback(intensityResponse.intensity);  
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

void ControlModule::getLedTemperature(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback) {
    App_messages::LED_panel::Temperature_request getTemperatureReq;

    uint32_t requestCanId = createCanId(getTemperatureReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t responseCanId = createCanId(App_messages::LED_panel::Temperature_response(0.0f).Type(), module, Codes::Instance::Exclusive, false);

    double timeoutSeconds = 2;

    manager.addRequest(requestCanId, getTemperatureReq.Export_data(), responseCanId, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::LED_panel::Temperature_response temperatureResponse(0.0f);
            if (temperatureResponse.Interpret_data(dataCopy)) {
                callback(temperatureResponse.temperature);  
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

void ControlModule::getHeaterTargetTemperature(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback) {
    App_messages::Heater::Get_target_temperature_request getTargetTempReq;

    uint32_t requestCanId = createCanId(getTargetTempReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t responseCanId = createCanId(App_messages::Heater::Get_target_temperature_response(0.0f).Type(), module, Codes::Instance::Exclusive, false);

    double timeoutSeconds = 2;

    manager.addRequest(requestCanId, getTargetTempReq.Export_data(), responseCanId, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::Heater::Get_target_temperature_response targetTempResponse(0.0f);
            if (targetTempResponse.Interpret_data(dataCopy)) {
                callback(targetTempResponse.temperature);  
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

void ControlModule::getHeaterIntensity(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback) {
    App_messages::Heater::Get_intensity_request getIntensityReq;

    uint32_t requestCanId = createCanId(getIntensityReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t responseCanId = createCanId(App_messages::Heater::Get_intensity_response(0.0f).Type(), module, Codes::Instance::Exclusive, false);

    double timeoutSeconds = 2;

    manager.addRequest(requestCanId, getIntensityReq.Export_data(), responseCanId, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::Heater::Get_intensity_response intensityResponse(0.0f);
            if (intensityResponse.Interpret_data(dataCopy)) {
                callback(intensityResponse.intensity);  
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

void ControlModule::getHeaterPlateTemperature(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback) {
    App_messages::Heater::Get_plate_temperature_request getPlateTempReq;

    uint32_t requestCanId = createCanId(getPlateTempReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t responseCanId = createCanId(App_messages::Heater::Get_plate_temperature_response(0.0f).Type(), module, Codes::Instance::Exclusive, false);

    double timeoutSeconds = 2;

    manager.addRequest(requestCanId, getPlateTempReq.Export_data(), responseCanId, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::Heater::Get_plate_temperature_response plateTempResponse(0.0f);
            if (plateTempResponse.Interpret_data(dataCopy)) {
                callback(plateTempResponse.temperature);
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

void ControlModule::setHeaterIntensity(Codes::Module module, float intensity, std::function<void(bool)> callback) {
    App_messages::Heater::Set_intensity setIntensity(intensity);

    uint32_t intensityCanId = createCanId(setIntensity.Type(), module, Codes::Instance::Exclusive, false);

    m_canRequestManager.sendWithoutResponse(intensityCanId, setIntensity.Export_data(), [callback](bool success) {
        callback(success);
    });
}

void ControlModule::setHeaterTargetTemperature(Codes::Module module, float targetTemperature, std::function<void(bool)> callback) {
    App_messages::Heater::Set_target_temperature setTargetTemperature(targetTemperature);

    uint32_t targetTempCanId = createCanId(setTargetTemperature.Type(), module, Codes::Instance::Exclusive, false);

    m_canRequestManager.sendWithoutResponse(targetTempCanId, setTargetTemperature.Export_data(), [callback](bool success) {
        callback(success);
    });
}
