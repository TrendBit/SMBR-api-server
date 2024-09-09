#pragma once

#include <vector>
#include <functional>
#include "can/CanRequestManager.hpp"
#include "can/CanMessage.hpp"
#include "can/CanIdGenerator.hpp"
#include "codes/codes.hpp"
#include <boost/asio.hpp>

/**
 * @class SystemModule
 * @brief Class responsible for managing system-level CAN bus communications.
 * 
 * The SystemModule class provides functions for communicating with different modules
 * on the system over the CAN bus, including probing available modules.
 */
class SystemModule {
public:
    /**
     * @brief Get the instance of SystemModule.
     * 
     * @param io_context Reference to Boost ASIO io_context for asynchronous operations.
     * @param canRequestManager Reference to CanRequestManager for sending CAN bus requests.
     * @return SystemModule& Singleton instance of SystemModule.
     */
    static SystemModule& getInstance(boost::asio::io_context& io_context, CanRequestManager& canRequestManager);

    /**
     * @brief Probe available modules on the system and retrieve their CAN message responses.
     * 
     * Sends a CAN request to discover available modules and collects their responses.
     * The results are passed to the provided callback function.
     * 
     * @param callback Function that receives a vector of CanMessage objects containing the module responses.
     */
    void getAvailableModules(std::function<void(const std::vector<CanMessage>&)> callback);

private:
    /**
     * @brief Constructor for SystemModule.
     * 
     * @param io_context Reference to Boost ASIO io_context for asynchronous operations.
     * @param canRequestManager Reference to CanRequestManager for managing CAN bus requests.
     */
    SystemModule(boost::asio::io_context& io_context, CanRequestManager& canRequestManager);

    boost::asio::io_context& m_ioContext; 
    CanRequestManager& m_canRequestManager; 
};

