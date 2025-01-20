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
#include "codes/messages/heater/set_intensity.hpp"
#include "codes/messages/heater/set_target_temperature.hpp"
#include "codes/messages/heater/turn_off.hpp"
#include "codes/messages/cuvette_pump/set_speed.hpp"
#include "codes/messages/cuvette_pump/get_speed_request.hpp"
#include "codes/messages/cuvette_pump/get_speed_response.hpp"
#include "codes/messages/cuvette_pump/set_flowrate.hpp"
#include "codes/messages/cuvette_pump/get_flowrate_request.hpp"
#include "codes/messages/cuvette_pump/get_flowrate_response.hpp"
#include "codes/messages/cuvette_pump/move.hpp"
#include "codes/messages/cuvette_pump/prime.hpp"
#include "codes/messages/cuvette_pump/purge.hpp"
#include "codes/messages/cuvette_pump/stop.hpp"
#include "codes/messages/aerator/set_speed.hpp"
#include "codes/messages/aerator/get_speed_request.hpp"
#include "codes/messages/aerator/get_speed_response.hpp"
#include "codes/messages/aerator/set_flowrate.hpp"


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
     * @brief Sets the intensity of heating or cooling.
     * 
     * @param module Target module.
     * @param intensity Value between -1.0 (cooling) and 1.0 (heating).
     * @param callback Callback function to indicate success or failure.
     */
    void setHeaterIntensity(Codes::Module module, float intensity, std::function<void(bool)> callback);

    /**
     * @brief Retrieves the current intensity of heating or cooling.
     * 
     * @param module Target module from which the heater intensity will be retrieved.
     * @param callback Callback function to handle the retrieved intensity.
     */
    void getHeaterIntensity(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback);

    /**
     * @brief Sets the target temperature for the heater.
     * 
     * @param module Target module.
     * @param targetTemperature Desired target temperature in ˚C.
     * @param callback Callback function to indicate success or failure.
     */
    void setHeaterTargetTemperature(Codes::Module module, float targetTemperature, std::function<void(bool)> callback);

    /**
     * @brief Retrieves the currently set target temperature for the heater.
     * 
     * @param module Target module from which the heater target temperature will be retrieved.
     * @param callback Callback function to handle the retrieved temperature.
     */
    void getHeaterTargetTemperature(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback);


    /**
     * @brief Retrieves the current temperature of the heater plate (metal heatspreader).
     *
     * @param module Target module from which the heater plate temperature will be retrieved.
     * @param callback Callback function to handle the retrieved temperature.
     */
    void getHeaterPlateTemperature(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback);

    /**
     * @brief Turns off the heater by setting intensity to 0.0 and disabling temperature regulation.
     * 
     * @param module Target module.
     * @param callback Callback function to indicate success or failure.
    */
    void turnOffHeater(Codes::Module module, std::function<void(bool)> callback);

    /**
     * @brief Sets the speed of the cuvette pump.
     * 
     * @param module Target module.
     * @param speed Value between -1.0 (out) and 1.0 (in).
     * @param callback Callback function to indicate success or failure.
     */
    void setCuvettePumpSpeed(Codes::Module module, float speed, std::function<void(bool)> callback);

    /**
     * @brief Retrieves the current speed of the cuvette pump.
     * 
     * @param module Target module from which the pump speed will be retrieved.
     * @param callback Callback function to handle the retrieved speed.
     */
    void getCuvettePumpSpeed(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback);

    /**
     * @brief Sets the flowrate of the cuvette pump.
     * 
     * @param module Target module.
     * @param flowrate Flowrate value in ml/min, can range from -1000.0 (out) to 1000.0 (in).
     * @param callback Callback function to indicate success or failure.
     */
    void setCuvettePumpFlowrate(Codes::Module module, float flowrate, std::function<void(bool)> callback);

    /**
     * @brief Retrieves the current flowrate of the cuvette pump.
     * 
     * @param manager CAN request manager used for communication.
     * @param module Target module from which the pump flowrate will be retrieved.
     * @param callback Callback function to handle the retrieved flowrate.
     */
    void getCuvettePumpFlowrate(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback);

    /**
     * @brief Moves the requested amount of liquid in or out of the cuvette.
     * 
     * @param module Target module.
     * @param volume Amount of liquid to move (ml). Positive for in, negative for out.
     * @param flowrate Flowrate of the pump (ml/min). If zero, the pump will use its maximum flowrate.
     * @param callback Callback function to indicate success or failure.
     */
    void moveCuvettePump(Codes::Module module, float volume, float flowrate, std::function<void(bool)> callback);

    /**
     * @brief Sends a command to prime the cuvette pump.
     * 
     * @param module Target module.
     * @param callback Callback function to indicate success or failure.
     */
    void primeCuvettePump(Codes::Module module, std::function<void(bool)> callback);

    /**
     * @brief Sends a command to purge the cuvette pump.
     * 
     * @param module Target module.
     * @param callback Callback function to indicate success or failure.
     */
    void purgeCuvettePump(Codes::Module module, std::function<void(bool)> callback);

    /**
     * @brief Sends a command to stop the cuvette pump and disable any planned movements.
     * 
     * @param module Target module.
     * @param callback Callback function to indicate success or failure.
     */
    void stopCuvettePump(Codes::Module module, std::function<void(bool)> callback);

    /**
     * @brief Sets the speed of the aerator.
     * 
     * @param module Target module.
     * @param speed Value between 0.0 and 1.0.
     * @param callback Callback function to indicate success or failure.
     */
    void setAeratorSpeed(Codes::Module module, float speed, std::function<void(bool)> callback);

    /**
     * @brief Retrieves the current speed of the aerator.
     * 
     * @param module Target module from which the aerator speed will be retrieved.
     * @param callback Callback function to handle the retrieved speed.
     */
    void getAeratorSpeed(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback);

    /**
     * @brief Sets the flowrate of the aerator.
     * 
     * @param module Target module.
     * @param flowrate Value between 10.0 and 5000.0 ml/min.
     * @param callback Callback function to indicate success or failure.
     */
    void setAeratorFlowrate(Codes::Module module, float flowrate, std::function<void(bool)> callback);

protected:
    boost::asio::io_context& m_ioContext; 
    CanRequestManager& m_canRequestManager; 
};

