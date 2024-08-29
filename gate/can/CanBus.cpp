#include "CanBus.hpp"
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <iomanip> 

CanBus::CanBus(boost::asio::io_context& io_context)
    : socket(io_context), ioContext(io_context)  {

    socketFd = ::socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (socketFd < 0) {
        perror("Socket");
        throw std::runtime_error("Failed to create CAN socket");
    }

    std::strncpy(ifr.ifr_name, "can0", IFNAMSIZ);
    if (ioctl(socketFd, SIOCGIFINDEX, &ifr) < 0) {
        perror("ioctl");
        close(socketFd);
        throw std::runtime_error("Failed to retrieve CAN interface index");
    }

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(socketFd, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) < 0) {
        perror("Bind");
        close(socketFd);
        throw std::runtime_error("Failed to bind CAN socket");
    }

    socket.assign(socketFd);
}

void CanBus::asyncSend(const CanMessage& message, std::function<void(bool)> handler) {
    std::cout << "CanBus::asyncSend - Start" << std::endl;
    std::cout << "CanBus::asyncSend - CAN ID: 0x" 
              << std::hex << std::setw(8) << std::setfill('0') << message.getId() << std::endl;

    struct can_frame frame;

    std::cout << "CanBus::asyncSend - Setting up CAN frame" << std::endl;
    frame.can_id = message.getId() | CAN_EFF_FLAG;
    frame.can_dlc = message.getData().size();
    std::cout << "CanBus::asyncSend - CAN frame DLC: " << static_cast<int>(frame.can_dlc) << std::endl;
    std::memcpy(frame.data, message.getData().data(), frame.can_dlc);
    std::cout << "CanBus::asyncSend - CAN frame data copied" << std::endl;

    std::cout << "CanBus::asyncSend - Calling async_write" << std::endl;
    boost::asio::async_write(socket, boost::asio::buffer(&frame, sizeof(frame)),
        [handler](boost::system::error_code ec, std::size_t bytes_transferred) {
            std::cout << "CanBus::asyncSend - async_write handler entered" << std::endl;
            std::cout << "CanBus::asyncSend - Bytes transferred: " << bytes_transferred << std::endl;

            if (ec) {
                std::cout << "CanBus::asyncSend - Error: " << ec.message() << std::endl;
            } else {
                std::cout << "CanBus::asyncSend - Success" << std::endl;
            }
            handler(!ec);
        }
    );
    std::cout << "CanBus::asyncSend - async_write called" << std::endl;
}




void CanBus::asyncReceive(std::function<void(bool, const CanMessage&)> handler) {
    std::cout << "CanBus::asyncReceive - Start" << std::endl;

    auto frame = std::make_shared<struct can_frame>();

    socket.async_read_some(boost::asio::buffer(frame.get(), sizeof(*frame)),
        [this, frame, handler](boost::system::error_code ec, std::size_t) {
            std::cout << "CanBus::asyncReceive - async_read_some completed, success: " << !ec << std::endl;

            if (ec) {
                std::cout << "CanBus::asyncReceive - Error: " << ec.message() << std::endl;
                handler(false, CanMessage(0, {}));
            } else {
                std::vector<uint8_t> data(frame->data, frame->data + frame->can_dlc);
                CanMessage message(frame->can_id & CAN_EFF_MASK, data);
                std::cout << "CanBus::asyncReceive - Success, CAN ID: 0x" 
                          << std::hex << std::setw(8) << std::setfill('0') << message.getId() 
                          << " Data Size: " << std::dec << data.size() << std::endl;
                handler(true, message);
            }
        }
    );
}
