/**
 * @file CommonModule.hpp
 * @author Vojtěch Mucha
 * @version 0.1
 * @date 23.08.2024
 */

#pragma once

#include <cstdint>
#include "codes/codes.hpp"

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

protected:
    Codes::Module module; ///< The module identifier.
    Codes::Instance instance; ///< The instance identifier.
};
