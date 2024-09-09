#ifndef CANBUS_HPP
#define CANBUS_HPP

#include <boost/asio.hpp>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <functional>
#include <vector>
#include <memory>
#include "CanMessage.hpp"

/**
 * @class CanBus
 * @brief Class responsible for CAN bus communication.
 * 
 * CanBus manages sending and receiving CAN messages asynchronously
 * using Boost Asio for asynchronous I/O operations.
 */
class CanBus {
public:
    /**
     * @brief Constructor for CanBus.
     * 
     * Initializes the CAN bus on the specified interface (can0) and prepares it for communication.
     * 
     * @param io_context Reference to Boost ASIO io_context for asynchronous operations.
     */
    CanBus(boost::asio::io_context& io_context);

    /**
     * @brief Destructor for CanBus.
     * 
     * Closes the CAN socket and cleans up resources.
     */
    ~CanBus();

    /**
     * @brief Asynchronously send a CAN message.
     * 
     * Sends the specified CAN message and calls the handler once the operation is complete.
     * 
     * @param message The CAN message to send.
     * @param handler Callback function to handle the result of the send operation.
     */
    void asyncSend(const CanMessage& message, std::function<void(bool)> handler);

    /**
     * @brief Asynchronously receive CAN messages.
     * 
     * Continuously listens for incoming CAN messages and calls the handler with each message received.
     * 
     * @param handler Callback function to handle the received messages.
     */
    void asyncReceive(std::function<void(bool, const CanMessage&)> handler);

private:
    /**
     * @brief Handle the result of the write operation.
     * 
     * @param error Boost system error code.
     * @param frame CAN frame that was sent.
     */
    void handleWrite(const boost::system::error_code& error, const struct can_frame& frame);

    /**
     * @brief Handle the result of the read operation.
     * 
     * @param error Boost system error code.
     * @param bytes_transferred Number of bytes transferred.
     * @param frame CAN frame that was received.
     */
    void handleRead(const boost::system::error_code& error, std::size_t bytes_transferred, const struct can_frame& frame);

    boost::asio::posix::stream_descriptor socket;
    int socketFd;
    struct sockaddr_can addr; 
    struct ifreq ifr;
    boost::asio::io_context& ioContext; 
};

#endif 
