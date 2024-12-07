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












