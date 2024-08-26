/**
 * @file SystemModule.hpp
 * @author Vojtěch Mucha
 * @version 0.1
 * @date 26.08.2024
 */

#pragma once

#include <vector>
#include <string>
#include <optional>
#include "can/CanRequestManager.hpp"
#include "can/CanIdGenerator.hpp"

/**
 * @class SystemModule
 * @brief Class representing the system module, providing information about available modules and system temperature.
 */
class SystemModule {
public:
    /**
     * @brief Provides access to the singleton instance of SystemModule.
     * @return Reference to the SystemModule instance.
     */
    static SystemModule& getInstance();

    /**
     * @brief Retrieves a list of available modules on the device.
     * @return A list of modules as an optional vector of bytes. Returns an empty value if the request fails.
     */
    std::optional<std::vector<uint8_t>> getAvailableModules();

    /**
     * @brief Retrieves the system temperature.
     * @return The system temperature as a float value.
     */
    float getSystemTemperature();

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
     * @brief Deleted copy constructor to enforce the singleton pattern.
     */
    SystemModule(const SystemModule&) = delete;

    /**
     * @brief Deleted assignment operator to enforce the singleton pattern.
     */
    SystemModule& operator=(const SystemModule&) = delete;
};

