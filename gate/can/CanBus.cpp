#include "CanBus.hpp"
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <functional>
#include <memory>

CanBus::CanBus(boost::asio::io_context& io_context)
    : socket(io_context), ioContext(io_context) {

    socketFd = ::socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (socketFd < 0) {
        perror("Socket");
        throw std::runtime_error("Failed to create CAN socket");
    }

    std::memset(&ifr, 0, sizeof(ifr));
    std::strncpy(ifr.ifr_name, "can0", IFNAMSIZ);

    if (ioctl(socketFd, SIOCGIFINDEX, &ifr) < 0) {
        perror("ioctl");
        close(socketFd);
        throw std::runtime_error("Failed to retrieve CAN interface index");
    }

    if (ifr.ifr_ifindex == 0) {
        std::cerr << "Invalid interface index" << std::endl;
        close(socketFd);
        throw std::runtime_error("Invalid interface index");
    }

    std::memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(socketFd, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) < 0) {
        perror("Bind");
        close(socketFd);
        throw std::runtime_error("Failed to bind CAN socket");
    }

    socket.assign(socketFd);
}

CanBus::~CanBus() {
    if (socketFd >= 0) {
        close(socketFd);
        std::cout << "CanBus socket closed" << std::endl;
    }
}

void CanBus::asyncSend(const CanMessage& message, std::function<void(bool)> handler) {
    auto frame = std::make_shared<struct can_frame>();
    frame->can_id = message.getId() | CAN_EFF_FLAG;
    frame->can_dlc = message.getData().size();
    std::memcpy(frame->data, message.getData().data(), frame->can_dlc);

    boost::asio::async_write(socket,
        boost::asio::buffer(frame.get(), sizeof(*frame)),
        [this, frame, handler](const boost::system::error_code& ec, std::size_t ) {
            if (ec) {
                std::cerr << "Failed to send CAN message with ID: 0x" << std::hex << (frame->can_id & CAN_EFF_MASK) << " and data: ";
                for (size_t i = 0; i < frame->can_dlc; ++i) {
                    std::cerr << std::hex << static_cast<int>(frame->data[i]) << " ";
                }
                std::cerr << std::endl;
            }
            handler(!ec);
        }
    );
}

void CanBus::asyncReceive(std::function<void(bool, const CanMessage&)> handler) {
    auto frame = std::make_shared<struct can_frame>();

    socket.async_read_some(boost::asio::buffer(frame.get(), sizeof(*frame)),
        [this, frame, handler](const boost::system::error_code& ec, std::size_t bytes_transferred) {
            if (!ec) {
                std::vector<uint8_t> data(frame->data, frame->data + frame->can_dlc);
                CanMessage message(frame->can_id & CAN_EFF_MASK, data);
                handler(true, message);
            } else {
                std::cerr << "Failed to receive CAN message" << std::endl;
                handler(false, CanMessage(0, {}));
            }

            asyncReceive(handler);
        }
    );
}