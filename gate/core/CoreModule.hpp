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
#include "codes/messages/core/serial_request.hpp"
#include "codes/messages/core/serial_response.hpp"
#include "codes/messages/core/supply_type_request.hpp"
#include "codes/messages/core/supply_type_response.hpp"


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

    /**
     * @brief Retrieves the serial number of the device.
     * 
     * @param manager Reference to the CAN request manager, responsible for managing requests and responses.
     * @param module Target module from which the serial number will be retrieved.
     * @param callback Function to handle the retrieved serial number as an int64_t.
     */
    void getSerialNumber(CanRequestManager& manager, Codes::Module module, std::function<void(int64_t)> callback);

    /**
     * @brief Retrieves the type of power supply powering the device.
     * 
     * @param manager Reference to the CAN request manager, responsible for managing requests and responses.
     * @param module Target module from which the power supply type will be retrieved.
     * @param callback Function to handle the retrieved power supply types (VIN, PoE, PoE_HB) as booleans.
     */
    void getPowerSupplyType(CanRequestManager& manager, Codes::Module module, std::function<void(bool, bool, bool, bool)> callback);









protected:
    boost::asio::io_context& m_ioContext; 
    CanRequestManager& m_canRequestManager; 
};