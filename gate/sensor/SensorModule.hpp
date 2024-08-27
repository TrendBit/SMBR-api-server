#pragma once

#include "base/CommonModule.hpp"
#include <cstring> 

/**
 * @class SensorModule
 * @brief Singleton class representing the sensor module in the system.
 * 
 * This class inherits from CommonModule and provides specific functionality for the sensor module.
 */
class SensorModule : public CommonModule {
public:
    /**
     * @brief Provides access to the singleton instance of SensorModule.
     * 
     * This method ensures that there is only one instance of SensorModule.
     * 
     * @return Reference to the singleton instance of SensorModule.
     */
    static SensorModule& getInstance();

    /**
     * @brief Retrieves the temperature from the top sensor on the cultivation bottle.
     * 
     * @return The temperature from the top sensor, or -1.0f if the request failed.
     */
    float getTopTemperature();

    /**
     * @brief Retrieves the temperature from the bottom sensor on the cultivation bottle.
     * 
     * @return The temperature from the bottom sensor, or -1.0f if the request failed.
     */
    float getBottomTemperature();

private:
    /**
     * @brief Private constructor to enforce the singleton pattern.
     * 
     * The constructor is private to prevent direct instantiation of the class.
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

    /**
     * @brief Default destructor.
     */
    ~SensorModule() = default;
};
