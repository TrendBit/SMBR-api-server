#include "ControlModule.hpp"



ControlModule::ControlModule(boost::asio::io_context& io_context, CanRequestManager& canRequestManager)
    : m_ioContext(io_context), m_canRequestManager(canRequestManager) {}


void ControlModule::setIntensity(Codes::Module module, float intensity, int channel, std::function<void(bool)> callback) {

    App_messages::LED_set_intensity set_intensity((uint8_t)channel, (float)intensity);

    uint32_t intensity_can_id = createCanId(set_intensity.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t intensity_response_id = createCanId(set_intensity.Type(), module, Codes::Instance::Undefined, false);

    double timeoutSeconds = 0;

    m_canRequestManager.addRequest(intensity_can_id, set_intensity.Export_data(), intensity_response_id, [callback](CanRequestStatus status, const CanMessage& response) {
        callback(true);
    }, timeoutSeconds);
}







