#include "MyCustomApi.h"
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <utility>
#include <cmath>

namespace OpenAPI {



MyCustomApi::MyCustomApi(const int timeOut)
    : OAIDefaultApi(timeOut) {
    std::thread([this] {
        while (true) {
            int s;
            struct sockaddr_can addr;
            struct ifreq ifr;
            struct can_frame frame;

            s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
            if (s < 0) {
                perror("Socket");
                return;
            }

            strcpy(ifr.ifr_name, "can0");
            ioctl(s, SIOCGIFINDEX, &ifr);

            addr.can_family = AF_CAN;
            addr.can_ifindex = ifr.ifr_ifindex;

            if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
                perror("Bind");
                close(s);
                return;
            }

            while (true) {
                if (read(s, &frame, sizeof(frame)) < 0) {
                    perror("Read");
                    close(s);
                    return;
                }

                std::ostringstream oss;
                oss << std::hex << std::setfill('0');
                oss << "ID: " << std::setw(3) << frame.can_id << " Data: ";
                for (int i = 0; i < frame.can_dlc; ++i) {
                    oss << std::setw(2) << static_cast<int>(frame.data[i]) << " ";
                }
                
                {
                    std::lock_guard<std::mutex> lock(lastMessageMutex);
                    lastPeriodicMessage = oss.str();
                }
            }
        }
    }).detach();
}

MyCustomApi::~MyCustomApi() {
}


uint32_t MyCustomApi::createCanId(Codes::Module module, Codes::Instance instance, Codes::Message_type messageType, bool emergencyFlag) {
    uint32_t id = 0;
    id |= (emergencyFlag ? 1 : 0) << 28; // 1 bit Emergency flag
    id |= static_cast<uint32_t>(messageType) << 16; // 12 bits Message type
    id |= static_cast<uint32_t>(module) << 4; // 8 bits Module ID
    id |= static_cast<uint32_t>(instance); // 4 bits Instance ID
    return id;
}


std::string MyCustomApi::ping() {
    std::string response;
    uint8_t data = 0x04;


    uint32_t can_id = createCanId(Codes::Module::Control_board, Codes::Instance::Exclusive, Codes::Message_type::Ping_request, false);

    if (sendCanMessage(can_id, &data, 1) && receiveNextCanMessage(response)) {
        return response;
    } else {
        return "CAN message failed";
    }
}


bool MyCustomApi::sendCanMessage(uint32_t can_id, const uint8_t* data, size_t data_len) {
    int s;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;

    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (s < 0) {
        perror("Socket");
        return false;
    }

    strcpy(ifr.ifr_name, "can0");
    ioctl(s, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Bind");
        close(s);
        return false;
    }

    frame.can_id = can_id | CAN_EFF_FLAG;
    frame.can_dlc = data_len;

    if (data && data_len > 0) {
        std::memcpy(frame.data, data, data_len);
    } else {
        std::memset(frame.data, 0, sizeof(frame.data));
    }

    if (write(s, &frame, sizeof(frame)) != sizeof(frame)) {
        perror("Write");
        close(s);
        return false;
    }

    close(s);
    return true;
}

bool MyCustomApi::receiveNextCanMessage(std::string &response) {
    int s;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;

    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (s < 0) {
        perror("Socket");
        return false;
    }

    strcpy(ifr.ifr_name, "can0");
    ioctl(s, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Bind");
        close(s);
        return false;
    }

    if (read(s, &frame, sizeof(frame)) < 0) {
        perror("Read");
        close(s);
        return false;
    }

    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    oss << "ID: " << std::setw(3) << frame.can_id << " Data: ";
    for (int i = 0; i < frame.can_dlc; ++i) {
        oss << std::setw(2) << static_cast<int>(frame.data[i]) << " ";
    }
    response = oss.str();

    close(s);
    return true;
}

} // namespace OpenAPI
