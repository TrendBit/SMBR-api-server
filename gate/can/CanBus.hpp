#ifndef CANBUS_HPP
#define CANBUS_HPP

#include "CanMessage.hpp"
#include <string>
#include <linux/can.h>
#include <net/if.h>

class CanBus {
public:
    CanBus();
    ~CanBus();
    bool send(const CanMessage& message);
    bool receive(CanMessage& message);

private:
    int socketFd;
    struct sockaddr_can addr;
    struct ifreq ifr;
};

#endif // CANBUS_HPP
