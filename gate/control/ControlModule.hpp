#pragma once

#include "base/CommonModule.hpp"
#include "can/CanRequestManager.hpp"
#include "can/CanIdGenerator.hpp"
#include <cstring>


/**
 * @class ControlModule
 * @brief Singleton class representing the control module in the system.
 * 
 * This class inherits from CommonModule and provides specific functionality for the control module.
 */
class ControlModule : public CommonModule {
public:
    /**
     * @brief Provides access to the singleton instance of ControlModule.
     * 
     * This method ensures that there is only one instance of ControlModule.
     * 
     * @return Reference to the singleton instance of ControlModule.
     */
    static ControlModule& getInstance();

    /**
     * @brief Sets the heater temperature for the control module.
     * @param temperature The target temperature to set.
     * @return True if the request was successful, false otherwise.
     */
    bool setHeaterTemperature(float temperature);

    /**
     * @brief Gets the current heater temperature from the control module.
     * @return The current heater temperature, or -1.0f if the request failed.
     */
    float getHeaterTemperature();

    /**
     * @brief Disables the heater in the control module.
     * @return True if the request was successful, false otherwise.
     */
    bool disableHeater();

    /**
     * @brief Default destructor for ControlModule.
     */
    ~ControlModule() = default;

private:
    /**
     * @brief Private constructor to enforce the singleton pattern.
     * 
     * The constructor is private to prevent direct instantiation of the class.
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
