#pragma once

#include <optional>
#include "base/CommonModule.hpp"
#include "can/CanRequestManager.hpp"
#include "can/CanIdGenerator.hpp"

/**
 * @class CoreModule
 * @brief Singleton class representing the core module in the system.
 * 
 * This class inherits from CommonModule and provides specific functionality for the core module.
 */
class CoreModule : public CommonModule {
public:
    /**
     * @brief Provides access to the singleton instance of CoreModule.
     * 
     * This method ensures that there is only one instance of CoreModule.
     * 
     * @return Reference to the singleton instance of CoreModule.
     */
    static CoreModule& getInstance();

    /**
     * @brief Sends a request to determine the power supply type (Adapter/PoE) and processes the response data.
     * @param adapter Reference to a boolean where the adapter status will be stored.
     * @param poe Reference to a boolean where the PoE status will be stored.
     * @return True if the request was successful and data was processed, false otherwise.
     */
    bool getSupplyType(bool& adapter, bool& poe);

    /**
     * @brief Default destructor for CoreModule.
     */
    ~CoreModule() = default;

private:
    /**
     * @brief Private constructor to enforce the singleton pattern.
     * 
     * The constructor is private to prevent direct instantiation of the class.
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
