#pragma once

#include "base/CommonModule.hpp"

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

    /**
     * @brief Default destructor.
     */
    ~CoreModule() = default;
};
