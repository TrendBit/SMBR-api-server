#include "ControlModule.hpp"



ControlModule::ControlModule(boost::asio::io_context& io_context, CanRequestManager& canRequestManager)
    : m_ioContext(io_context), m_canRequestManager(canRequestManager) {}


void ControlModule::setIntensity(Codes::Module module, float intensity, std::function<void(bool)> callback) {

    intensity = std::max(0.0f, std::min(intensity, 100.0f));

    int scaledValue = static_cast<int>((intensity / 100.0f) * 255); 

    int decimalPart = static_cast<int>((intensity - static_cast<int>(intensity)) * 255);

    std::vector<uint8_t> intensity_data = {
        0x03,                              
        static_cast<uint8_t>(scaledValue),  
        static_cast<uint8_t>(decimalPart)   
    };

    uint32_t intensity_can_id = createCanId(Codes::Message_type::LED_set_intensity, module, Codes::Instance::Exclusive, false);
    uint32_t intensity_response_id = createCanId(Codes::Message_type::LED_set_intensity, module, Codes::Instance::Undefined, false);

    int timeoutSeconds = 0;

    m_canRequestManager.addRequest(intensity_can_id, intensity_data, intensity_response_id, [callback](CanRequestStatus status, const CanMessage& response) {
        callback(true);  
    }, timeoutSeconds);
}




