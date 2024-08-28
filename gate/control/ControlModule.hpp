/**
 * @file ControlModule.hpp
 * @author Vojtěch Mucha
 * @version 0.1
 * @date 28.08.2024
 */

#pragma once

#include "can/CanRequestManager.hpp"
#include "can/CanIdGenerator.hpp"
#include "base/CommonModule.hpp"
#include <future>
#include <cstring>

/**
 * @class ControlModule
 * @brief Singleton class representing the control module in the system.
 * 
 * The ControlModule class inherits from CommonModule and provides specific functionality for controlling
 * the heater in the system. It supports asynchronous operations for setting and retrieving the heater temperature.
 */
class ControlModule : public CommonModule {
public:
    /**
     * @brief Retrieves the singleton instance of ControlModule.
     * 
     * This method ensures that only one instance of ControlModule exists throughout the application.
     * 
     * @return Reference to the singleton instance of ControlModule.
     */
    static ControlModule& getInstance();

    /**
     * @brief Asynchronously sets the heater temperature.
     * 
     * This method sends a request over the CAN bus to set the heater temperature and returns
     * a `std::future` containing a boolean indicating the success of the operation.
     * 
     * @param temperature The target temperature to set.
     * @return `std::future<bool>` indicating the success of the operation.
     */
    std::future<bool> setHeaterTemperature(float temperature);

    /**
     * @brief Asynchronously retrieves the current heater temperature.
     * 
     * This method sends a request over the CAN bus to get the current heater temperature and returns
     * a `std::future` containing the temperature as a float value.
     * 
     * @return `std::future<float>` containing the current heater temperature. If the request fails, the value will be negative.
     */
    std::future<float> getHeaterTemperature();

    /**
     * @brief Asynchronously disables the heater.
     * 
     * This method sends a request over the CAN bus to disable the heater and returns
     * a `std::future` containing a boolean indicating the success of the operation.
     * 
     * @return `std::future<bool>` indicating the success of the operation.
     */
    std::future<bool> disableHeater();

    /**
     * @brief Default destructor for ControlModule.
     */
    ~ControlModule() = default;

private:
    /**
     * @brief Private constructor to enforce the singleton pattern.
     */
    ControlModule();

    /**
     * @brief Deleted copy constructor to prevent copying of the singleton instance.
     */
    ControlModule(const ControlModule&) = delete;

    /**
     * @brief Deleted assignment operator to prevent copying of the singleton instance.
     * 
     * @return Reference to the ControlModule instance.
     */
    ControlModule& operator=(const ControlModule&) = delete;
};
