#ifndef CANBUS_HPP
#define CANBUS_HPP

#include <boost/asio.hpp>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <functional>
#include <vector>
#include <memory>
#include "CanMessage.hpp"

class CanBus {
public:
    CanBus(boost::asio::io_context& io_context);
    ~CanBus();

    void asyncSend(const CanMessage& message, std::function<void(bool)> handler);
    void asyncReceive(std::function<void(bool, const CanMessage&)> handler);

private:
    void handleWrite(const boost::system::error_code& error, const struct can_frame& frame);
    void handleRead(const boost::system::error_code& error, std::size_t bytes_transferred, const struct can_frame& frame);

    boost::asio::posix::stream_descriptor socket;
    int socketFd;
    struct sockaddr_can addr;
    struct ifreq ifr;
    boost::asio::io_context& ioContext;
};

#endif // CANBUS_HPP
