#ifndef DUMMYCANBUS_HPP
#define DUMMYCANBUS_HPP

#include <boost/asio.hpp>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <functional>
#include <vector>
#include <memory>
#include "CanMessage.hpp"
#include "ICanBus.hpp"

/**
 * @class DummyCanBus
 * @brief Class responsible for CAN bus communication.
 * 
 * CanBus manages sending and receiving CAN messages asynchronously
 * using Boost Asio for asynchronous I/O operations.
 */
class DummyCanBus : public ICanBus {
public:
    /**
     * @brief Constructor for CanBus.
     * 
     * Initializes the CAN bus on the specified interface (can0) and prepares it for communication.
     * 
     * @param io_context Reference to Boost ASIO io_context for asynchronous operations.
     */
    DummyCanBus(boost::asio::io_context& io_context);

    /**
     * @brief Destructor for CanBus.
     * 
     * Closes the CAN socket and cleans up resources.
     */
    virtual ~DummyCanBus();

    void asyncSend(const CanMessage& message, std::function<void(bool)> handler) override;

    void asyncReceive(std::function<void(bool, const CanMessage&)> handler) override;

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

private:
    boost::asio::io_context& ioContext; 
};

#endif 
