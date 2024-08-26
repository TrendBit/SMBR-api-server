/**
 * @file CommonModule.hpp
 * @author Vojtěch Mucha
 * @version 0.1
 * @date 23.08.2024
 */

#pragma once

#include <cstdint>
#include "codes/codes.hpp"
#include <string>

/**
 * @class CommonModule
 * @brief Base class for modules that interact with the CAN bus.
 */
class CommonModule {
public:
    /**
     * @brief Constructs a CommonModule object with the specified module and instance identifiers.
     * @param mod The module identifier.
     * @param inst The instance identifier.
     */
    CommonModule(Codes::Module mod, Codes::Instance inst);

    /**
     * @brief Virtual destructor for the CommonModule class.
     */
    virtual ~CommonModule();

    /**
     * @brief Sends a ping request to the associated module over the CAN bus.
     * @return The response time in milliseconds, or -1.0f if the request failed.
     */
    float ping();

    /**
     * @brief Sends a load request to the associated module over the CAN bus and processes the response data.
     * @param load Reference to a float where the load value will be stored.
     * @param cores Reference to an integer where the number of cores will be stored.
     * @return True if the request was successful and data was processed, false otherwise.
     */
    bool getLoad(float& load, int& cores);

    /**
     * @brief Sends a core temperature request to the associated module over the CAN bus and processes the response data.
     * @param core_temp Reference to a float where the core temperature value will be stored.
     * @return True if the request was successful and data was processed, false otherwise.
     */
    bool getCoreTemp(float& core_temp);

    /**
     * @brief Sends a restart request to the associated module over the CAN bus.
     * @param uid The UID of the module to be restarted.
     * @return True if the request was successful, false otherwise.
     */
    bool restart(const std::string& uid);

    /**
     * @brief Sends a bootloader request to the associated module over the CAN bus.
     * @param uid The UID of the module to be booted into bootloader mode.
     * @return True if the request was successful, false otherwise.
     */
    bool bootloader(const std::string& uid);

protected:
    Codes::Module module; ///< The module identifier.
    Codes::Instance instance; ///< The instance identifier.
};

