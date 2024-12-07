#pragma once

#include "can/CanRequestManager.hpp"
#include <functional>
#include <boost/asio.hpp>
#include "can/CanIdGenerator.hpp"
#include "codes/codes.hpp"
#include <iomanip>
#include <sstream>

#include "codes/messages/led_panel/set_intensity.hpp"

/**
 * @class ControlModule
 * @brief Handles control operations including setting intensity.
 */
class ControlModule {
public:
    ControlModule(boost::asio::io_context& io_context, CanRequestManager& canRequestManager);

    /**
     * @brief Asynchronously sets the intensity of LEDs on a module.
     * 
     * @param module Target module.
     * @param intensity Value between 0 and 1.
     * @param channel Channel to light up.
     */
    void setIntensity(Codes::Module module, float intensity, int channel, std::function<void(bool)> callback);

protected:
    boost::asio::io_context& m_ioContext; 
    CanRequestManager& m_canRequestManager; 
};
