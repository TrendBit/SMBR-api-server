/**
 * @file CoreModule.hpp
 * @author Vojtěch Mucha
 * @version 0.1
 * @date 28.08.2024
 */

#pragma once

#include "base/CommonModule.hpp"
#include "can/CanIdGenerator.hpp"
#include "can/CanRequestManager.hpp"
#include <future>

/**
 * @class CoreModule
 * @brief Singleton class representing the core module in the system.
 * 
 * The CoreModule class inherits from CommonModule and provides specific functionality for interacting with
 * the core module of the system, such as determining the power supply type.
 */
class CoreModule : public CommonModule {
public:
    /**
     * @brief Retrieves the singleton instance of CoreModule.
     * 
     * This method ensures that only one instance of CoreModule exists throughout the application.
     * 
     * @return Reference to the singleton instance of CoreModule.
     */
    static CoreModule& getInstance();

    /**
     * @brief Asynchronously retrieves the power supply type (Adapter/PoE).
     * 
     * This method sends a request over the CAN bus to determine the power supply type and returns
     * a `std::future` containing a boolean indicating the success of the operation. The results for adapter and PoE
     * are stored in the provided references.
     * 
     * @param adapter Reference to a boolean where the adapter status will be stored.
     * @param poe Reference to a boolean where the PoE status will be stored.
     * @return `std::future<bool>` indicating the success of the operation.
     */
    std::future<bool> getSupplyType(bool& adapter, bool& poe);

    /**
     * @brief Default destructor for CoreModule.
     */
    ~CoreModule() = default;

private:
    /**
     * @brief Private constructor to enforce the singleton pattern.
     */
    CoreModule();

    /**
     * @brief Deleted copy constructor to prevent copying of the singleton instance.
     */
    CoreModule(const CoreModule&) = delete;

    /**
     * @brief Deleted assignment operator to prevent copying of the singleton instance.
     * 
     * @return Reference to the CoreModule instance.
     */
    CoreModule& operator=(const CoreModule&) = delete;
};
