/**
 * @file SensorModule.hpp
 * @author Vojtěch Mucha
 * @version 0.1
 * @date 28.08.2024
 */

#pragma once

#include "base/CommonModule.hpp"
#include "can/CanIdGenerator.hpp"
#include "can/CanRequestManager.hpp"
#include <cstring>
#include <future>

/**
 * @class SensorModule
 * @brief Singleton class representing the sensor module in the system.
 * 
 * The SensorModule class inherits from CommonModule and provides specific functionality for retrieving temperature
 * data from sensors attached to the system. It supports asynchronous operations.
 */
class SensorModule : public CommonModule {
public:
    /**
     * @brief Retrieves the singleton instance of SensorModule.
     * 
     * This method ensures that only one instance of SensorModule exists throughout the application.
     * 
     * @return Reference to the singleton instance of SensorModule.
     */
    static SensorModule& getInstance();

    /**
     * @brief Asynchronously retrieves the temperature from the top sensor on the cultivation bottle.
     * 
     * This method sends a request over the CAN bus to get the temperature from the top sensor and returns
     * a `std::future` containing the temperature as a float value.
     * 
     * @return `std::future<float>` containing the top sensor temperature. If the request fails, the value will be negative.
     */
    std::future<float> getTopTemperature();

    /**
     * @brief Asynchronously retrieves the temperature from the bottom sensor on the cultivation bottle.
     * 
     * This method sends a request over the CAN bus to get the temperature from the bottom sensor and returns
     * a `std::future` containing the temperature as a float value.
     * 
     * @return `std::future<float>` containing the bottom sensor temperature. If the request fails, the value will be negative.
     */
    std::future<float> getBottomTemperature();

    /**
     * @brief Default destructor for SensorModule.
     */
    ~SensorModule() = default;

private:
    /**
     * @brief Private constructor to enforce the singleton pattern.
     */
    SensorModule();

    /**
     * @brief Deleted copy constructor to prevent copying of the singleton instance.
     */
    SensorModule(const SensorModule&) = delete;

    /**
     * @brief Deleted assignment operator to prevent copying of the singleton instance.
     * 
     * @return Reference to the SensorModule instance.
     */
    SensorModule& operator=(const SensorModule&) = delete;
};
