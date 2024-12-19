#pragma once

#include "can/CanRequestManager.hpp"
#include "codes/codes.hpp"
#include "can/CanIdGenerator.hpp"
#include <functional>
#include <boost/asio.hpp>

#include "codes/messages/common/ping_request.hpp"
#include "codes/messages/common/ping_response.hpp"
#include "codes/messages/common/core_temp_request.hpp"
#include "codes/messages/common/core_temp_response.hpp"
#include "codes/messages/common/device_reset.hpp"
#include "codes/messages/common/device_usb_bootloader.hpp"
#include "codes/messages/common/device_can_bootloader.hpp"
#include "codes/messages/common/core_load_request.hpp"
#include "codes/messages/common/core_load_response.hpp"


/**
 * @class CommonModule
 * @brief Represents a common module that can send ping requests and handle CAN communication.
 * 
 * This class is responsible for handling common CAN communication tasks like sending a ping request.
 */
class CommonModule {
public:
    /**
     * @brief Virtual destructor for the CommonModule class.
     */
    virtual ~CommonModule() = default;

    /**
     * @brief Constructs a CommonModule object.
     * 
     * @param io_context Reference to the Boost ASIO io_context used for asynchronous operations.
     * @param canRequestManager Reference to the CanRequestManager used for sending and receiving CAN requests.
     */
    CommonModule(boost::asio::io_context& io_context, CanRequestManager& canRequestManager);

    /**
     * @brief Sends a ping request to the specified CAN module with a sequence number.
     * 
     * This method sends a ping request to the module specified by the `module` parameter with the provided 
     * sequence number (`seq_num`), waits for a response, and calls the provided callback with the time 
     * taken for the ping or an error code.
     * 
     * @param manager Reference to the CanRequestManager used to manage CAN requests.
     * @param module The module to which the ping request is being sent (as specified by the `Codes::Module` enum).
     * @param seq_num Sequence number for the ping request.
     * @param callback Function to be called with the time (in milliseconds) for the ping response. 
     *                 A negative value indicates an error (-1 for failure, -2 for timeout).
     */
    void ping(CanRequestManager& manager, Codes::Module module, uint8_t seq_num, std::function<void(float)> callback);


    /**
    * @brief Sends a request to retrieve the CPU/MCU load from the specified CAN module.
    *
    * This method sends a request to the module specified by the `module` parameter to retrieve the CPU/MCU load.
    * The function waits for a response and calls the provided callback with the load value or an error code.
    *
    * @param manager Reference to the CanRequestManager used to manage CAN requests.
    * @param module The module from which the CPU/MCU load is being requested.
    * @param callback Function to be called with the CPU/MCU load value (between 0 and 1).
    */
    void getCoreLoad(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback);


    /**
     * @brief Sends a request to retrieve the core temperature from the specified CAN module.
     * 
     * This method sends a request to the module specified by the `module` parameter to retrieve the core temperature.
     * The function waits for a response and calls the provided callback with the temperature value or an error code.
     * 
     * @param manager Reference to the CanRequestManager used to manage CAN requests.
     * @param module The module from which the core temperature is being requested (as specified by the `Codes::Module` enum).
     * @param callback Function to be called with the core temperature value (in degrees Celsius).
     *                 A negative value indicates an error (-1 for failure, -2 for timeout).
     */
    void getCoreTemp(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback);

    /**
    * @brief Sends a request to restart the specified CAN module.
    *
    * This method sends a reset request to the module specified by the `module` parameter.
    *  
    * @param manager Reference to the CanRequestManager used to manage CAN requests.
    * @param module The module to be restarted.
    * @param callback Callback function that will be called upon completion.
    */
    void sendDeviceReset(CanRequestManager& manager, Codes::Module module, std::function<void(bool)> callback);

    /**
    * @brief Sends a request to reboot the specified CAN module into USB bootloader mode.
    *
    * This method sends a request to the specified module to enter USB bootloader mode.
    *  
    * @param manager Reference to the CanRequestManager used to manage CAN requests.
    * @param module The module to be rebooted in USB bootloader mode.
    * @param callback Callback function that will be called upon completion.
    */
    void sendDeviceUsbBootloader(CanRequestManager& manager, Codes::Module module, std::function<void(bool)> callback);

    /**
    * @brief Sends a request to reboot the specified CAN module into CAN bootloader mode.
    *
    * This method sends a request to the specified module to enter CAN bootloader mode.
    *  
    * @param manager Reference to the CanRequestManager used to manage CAN requests.
    * @param module The module to be rebooted in CAN bootloader mode.
    * @param callback Callback function that will be called upon completion.
    */
    void sendDeviceCanBootloader(CanRequestManager& manager, Codes::Module module, std::function<void(bool)> callback);






protected:
    boost::asio::io_context& m_ioContext; 
    CanRequestManager& m_canRequestManager; 
};
