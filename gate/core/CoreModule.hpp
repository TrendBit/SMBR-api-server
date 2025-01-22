#pragma once

#include "can/CanRequestManager.hpp"
#include <functional>
#include <boost/asio.hpp>
#include "can/CanIdGenerator.hpp"
#include "codes/codes.hpp"
#include <iomanip>

#include "codes/messages/core/sid_request.hpp"
#include "codes/messages/core/sid_response.hpp"
#include "codes/messages/core/ip_request.hpp"
#include "codes/messages/core/ip_response.hpp"
#include "codes/messages/core/hostname_request.hpp"
#include "codes/messages/core/hostname_response.hpp"


/**
 * @class CoreModule
 */
class CoreModule {
public:
    CoreModule(boost::asio::io_context& io_context, CanRequestManager& canRequestManager);

    /**
     * @brief Retrieves the short ID (SID) of the device.
     * 
     * @param manager Reference to the CAN request manager, responsible for managing requests and responses.
     * @param module Target module from which the SID will be retrieved.
     * @param callback Function to handle the retrieved SID as a string.
     */
    void getShortID(CanRequestManager& manager, Codes::Module module, std::function<void(std::string)> callback);

    /**
     * @brief Retrieves the IP address of the device.
     * 
     * @param manager Reference to the CAN request manager, responsible for managing requests and responses.
     * @param module Target module from which the IP address will be retrieved.
     * @param callback Callback function to handle the IP address or error message (empty string for failure).
     */
    void getIpAddress(CanRequestManager& manager, Codes::Module module, std::function<void(std::string)> callback);

    /**
     * @brief Retrieves the hostname of the device.
     * 
     * @param manager Reference to the CAN request manager, responsible for managing requests and responses.
     * @param module Target module from which the hostname will be retrieved.
     * @param callback Function to handle the retrieved hostname as a string.
     */
    void getHostname(CanRequestManager& manager, Codes::Module module, std::function<void(std::string)> callback);







protected:
    boost::asio::io_context& m_ioContext; 
    CanRequestManager& m_canRequestManager; 
};