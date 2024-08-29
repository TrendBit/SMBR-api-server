#ifndef CANBUS_HPP
#define CANBUS_HPP

#include "CanMessage.hpp"
#include <boost/asio.hpp>
#include <linux/can.h>
#include <linux/can/raw.h>

/**
 * @class CanBus
 * @brief Handles CAN bus communication via socketCAN with async operations using boost::asio.
 */
class CanBus {
public:
    /**
     * @brief Constructs a CanBus object and initializes the CAN interface.
     * @param io_context The io_context used for asynchronous operations.
     */
    CanBus(boost::asio::io_context& io_context);

    /**
     * @brief Sends a CAN message asynchronously.
     * @param message The CAN message to send.
     * @param handler A handler to be called once the operation is complete.
     */
    void asyncSend(const CanMessage& message, std::function<void(bool)> handler);

    /**
     * @brief Receives a CAN message asynchronously.
     * @param handler A handler to be called once a message is received.
     */
    void asyncReceive(std::function<void(bool, const CanMessage&)> handler);

private:
    int socketFd; ///< Socket file descriptor for CAN communication.
    boost::asio::posix::stream_descriptor socket; ///< Asio wrapper for CAN socket.
    struct ifreq ifr; ///< Interface request structure for CAN.
    struct sockaddr_can addr; ///< Socket address structure for CAN.
    boost::asio::io_context& ioContext; ///< Reference to the io_context used for asynchronous operations.
};

#endif // CANBUS_HPP
