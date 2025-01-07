#pragma once

#include "can/CanRequestManager.hpp"
#include <functional>
#include <boost/asio.hpp>
#include "can/CanIdGenerator.hpp"
#include "codes/codes.hpp"
#include <iomanip>
#include <sstream>

#include <iostream>

#include "codes/messages/led_panel/set_intensity.hpp"
#include "codes/messages/led_panel/get_intensity_request.hpp"
#include "codes/messages/led_panel/get_intensity_response.hpp"
#include "codes/messages/led_panel/temperature_request.hpp"
#include "codes/messages/led_panel/temperature_response.hpp"
#include "codes/messages/heater/get_target_temperature_request.hpp"
#include "codes/messages/heater/get_target_temperature_response.hpp"
#include "codes/messages/heater/get_intensity_request.hpp"
#include "codes/messages/heater/get_intensity_response.hpp"
#include "codes/messages/heater/get_plate_temperature_request.hpp"
#include "codes/messages/heater/get_plate_temperature_response.hpp"


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

    /**
     * @brief Retrieves the current intensity of LEDs for a specified module and channel.
     * 
     * @param manager Reference to the CAN request manager, responsible for managing requests and responses.
     * @param module Target module from which the LED intensity will be retrieved.
     * @param channel The specific LED channel whose intensity will be retrieved. Valid channels range from 0 to 3.
     */
    void getIntensity(CanRequestManager& manager, Codes::Module module, int channel, std::function<void(float)> callback);

    /**
     * @brief Retrieves the current temperature of LEDs for a specified module and channel.
     * 
     * @param module Target module from which the LED temperature will be retrieved.
     * @param callback Callback function to handle the retrieved temperature.
     */
    void getLedTemperature(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback); 

    /**
     * @brief Retrieves the currently set target temperature for the heater.
     * 
     * @param module Target module from which the heater target temperature will be retrieved.
     * @param callback Callback function to handle the retrieved temperature.
     */
    void getHeaterTargetTemperature(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback);

    /**
     * @brief Retrieves the current intensity of heating or cooling.
     * 
     * @param module Target module from which the heater intensity will be retrieved.
     * @param callback Callback function to handle the retrieved intensity.
     */
    void getHeaterIntensity(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback);

    /**
     * @brief Retrieves the current temperature of the heater plate (metal heatspreader).
     *
     * @param module Target module from which the heater plate temperature will be retrieved.
     * @param callback Callback function to handle the retrieved temperature.
     */
    void getHeaterPlateTemperature(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback);



protected:
    boost::asio::io_context& m_ioContext; 
    CanRequestManager& m_canRequestManager; 
};

