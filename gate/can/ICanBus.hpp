#ifndef ICANBUS_HPP
#define ICANBUS_HPP

#include <linux/can.h>
#include <linux/can/raw.h>
#include <functional>
#include <vector>
#include <memory>
#include "CanMessage.hpp"

/**
 * @class ICanBus
 * @brief interface for CAN bus communication.
 * 
 * CanBus manages sending and receiving CAN messages asynchronously
 * using Boost Asio for asynchronous I/O operations.
 */
class ICanBus {
public:
    
    virtual ~ICanBus();

    /**
     * @brief Asynchronously send a CAN message.
     * 
     * Sends the specified CAN message and calls the handler once the operation is complete.
     * 
     * @param message The CAN message to send.
     * @param handler Callback function to handle the result of the send operation.
     */
    virtual void asyncSend(const CanMessage& message, std::function<void(bool)> handler) = 0;

    /**
     * @brief Asynchronously receive CAN messages.
     * 
     * Continuously listens for incoming CAN messages and calls the handler with each message received.
     * 
     * @param handler Callback function to handle the received messages.
     */
    virtual void asyncReceive(std::function<void(bool, const CanMessage&)> handler) = 0;

};

#endif 
