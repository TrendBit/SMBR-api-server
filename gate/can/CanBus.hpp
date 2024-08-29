/**
 * @file CanBus.hpp
 * @author Vojtěch Mucha
 * @version 0.1
 * @date 23.08.2024
 */

#pragma once

#include "CanMessage.hpp"
#include <string>
#include <linux/can.h>
#include <net/if.h>

/**
 * @class CanBus
 * @brief Manages communication over the CAN bus.
 */
class CanBus {
public:
    /**
     * @brief Constructs a CanBus object and initializes the CAN bus interface.
     * @exception std::runtime_error Thrown if the CAN socket cannot be created or bound.
     */
    CanBus();

    /**
     * @brief Destructs the CanBus object and closes the CAN socket.
     */
    ~CanBus();

    /**
     * @brief Sends a CAN message over the CAN bus.
     * @param message The CanMessage object containing the message to send.
     * @return True if the message was successfully sent, false otherwise.
     */
    bool send(const CanMessage& message);

    /**
     * @brief Receives a CAN message from the CAN bus.
     * @param message The CanMessage object to store the received message.
     * @return True if a message was successfully received, false otherwise.
     */
    bool receive(CanMessage& message);

private:
    int socketFd; ///< File descriptor for the CAN socket.
    struct sockaddr_can addr; ///< Address structure for the CAN socket.
    struct ifreq ifr; ///< Interface request structure for the CAN interface.
};


