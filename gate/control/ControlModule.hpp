#pragma once

#include "base/CommonModule.hpp"

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

    /**
     * @brief Default destructor.
     */
    ~ControlModule() = default;
};
