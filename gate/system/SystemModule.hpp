/**
 * @file SystemModule.hpp
 * @author Vojtěch Mucha
 * @version 0.1
 * @date 28.08.2024
 */

#pragma once

#include <vector>
#include <string>
#include <optional>
#include <future>
#include "can/CanRequestManager.hpp"
#include "can/CanIdGenerator.hpp"

/**
 * @class SystemModule
 * @brief Singleton class representing the system module, providing information about available modules and system temperature.
 * 
 * The SystemModule class is responsible for querying available modules and retrieving the system's temperature.
 * It provides asynchronous methods to perform these operations.
 */
class SystemModule {
public:
    /**
     * @brief Retrieves the singleton instance of SystemModule.
     * 
     * This method ensures that only one instance of SystemModule exists throughout the application.
     * 
     * @return Reference to the SystemModule instance.
     */
    static SystemModule& getInstance();

    /**
     * @brief Asynchronously retrieves a list of available modules on the device.
     * 
     * This method sends a request over the CAN bus to determine which modules are available on the device
     * and returns a `std::future` containing an optional vector of bytes representing the module data.
     * 
     * @return `std::future` containing an `std::optional<std::vector<uint8_t>>` with the module data.
     * The optional will be empty if the request fails.
     */
    std::future<std::optional<std::vector<uint8_t>>> getAvailableModules();

    /**
     * @brief Asynchronously retrieves the system temperature.
     * 
     * This method sends a request over the CAN bus to retrieve the system's temperature and returns a `std::future`
     * containing the temperature as a float value.
     * 
     * @return `std::future<float>` containing the system temperature. If the request fails, the value will be negative.
     */
    std::future<float> getSystemTemperature();

    /**
     * @brief Default destructor for SystemModule.
     */
    ~SystemModule() = default;

private:
    /**
     * @brief Private constructor to enforce the singleton pattern.
     */
    SystemModule() = default;

    /**
     * @brief Deleted copy constructor to prevent copying of the singleton instance.
     */
    SystemModule(const SystemModule&) = delete;

    /**
     * @brief Deleted assignment operator to prevent copying of the singleton instance.
     * 
     * @return Reference to the SystemModule instance.
     */
    SystemModule& operator=(const SystemModule&) = delete;
};
