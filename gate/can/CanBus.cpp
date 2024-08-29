#include "CanBus.hpp"
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

CanBus::CanBus() : socketFd(-1) {
    socketFd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
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
}

CanBus::~CanBus() {
    if (socketFd >= 0) {
        close(socketFd);
    }
}

bool CanBus::send(const CanMessage& message) {
    struct can_frame frame;
    frame.can_id = message.getId() | CAN_EFF_FLAG;
    frame.can_dlc = message.getData().size();
    std::memcpy(frame.data, message.getData().data(), frame.can_dlc);

    if (write(socketFd, &frame, sizeof(frame)) != sizeof(frame)) {
        perror("Write");
        return false;
    }

    return true;
}

bool CanBus::receive(CanMessage& message) {
    struct can_frame frame;
    if (read(socketFd, &frame, sizeof(frame)) < 0) {
        perror("Read");
        return false;
    }

    std::vector<uint8_t> data(frame.data, frame.data + frame.can_dlc);
    message = CanMessage(frame.can_id & CAN_EFF_MASK, data);
    return true;
}
