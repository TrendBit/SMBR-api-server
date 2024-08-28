#pragma once

#include <cstdint>
#include <string>
#include <future>
#include "codes/codes.hpp"

/**
 * @class CommonModule
 * @brief Base class for modules that interact with the CAN bus.
 */
class CommonModule {
public:
    CommonModule(Codes::Module mod, Codes::Instance inst);
    virtual ~CommonModule();

    /**
     * @brief Sends a ping request to the associated module over the CAN bus.
     * @return The response time in milliseconds, or -1.0f if the request failed.
     */
    std::future<float> ping();

    /**
     * @brief Sends a load request to the associated module over the CAN bus and processes the response data.
     * @param load Reference to a float where the load value will be stored.
     * @param cores Reference to an integer where the number of cores will be stored.
     * @return A future that will contain true if the request was successful and data was processed, false otherwise.
     */
    std::future<bool> getLoad(float& load, int& cores);

    /**
     * @brief Sends a core temperature request to the associated module over the CAN bus and processes the response data.
     * @param core_temp Reference to a float where the core temperature value will be stored.
     * @return A future that will contain true if the request was successful and data was processed, false otherwise.
     */
    std::future<bool> getCoreTemp(float& core_temp);

    std::future<bool> restart(const std::string& uid);
    std::future<bool> bootloader(const std::string& uid);

protected:
    Codes::Module module;
    Codes::Instance instance;
};
