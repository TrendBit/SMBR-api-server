#include "can/CanHandler.hpp"
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <string>
#include <utility>

CanHandler::CanHandler() {}
CanHandler::~CanHandler() {}

std::pair<bool, std::string> CanHandler::sendPingRequest(uint32_t can_id, uint8_t seq_number) {
    std::string response;
    uint8_t data = seq_number;  

    if (sendCanMessage(can_id, &data, 1) && receiveNextCanMessage(response)) {
        return std::make_pair(true, response);
    } else {
        return std::make_pair(false, "CAN message failed");
    }
}

bool CanHandler::sendCanMessage(uint32_t can_id, const uint8_t* data, size_t data_len) {
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

bool CanHandler::receiveNextCanMessage(std::string &response) {
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
    oss << "ID: " << std::setw(8) << frame.can_id << " Data: ";
    for (int i = 0; i < frame.can_dlc; ++i) {
        oss << std::setw(2) << static_cast<int>(frame.data[i]) << " ";
    }
    response = oss.str();

    close(s);
    return true;
}

uint32_t CanHandler::extractCanIdFromResponse(const std::string& response) {
    std::size_t idPos = response.find("ID: ");
    if (idPos == std::string::npos) {
        return 0;
    }

    std::size_t idStart = idPos + 4;
    std::size_t idEnd = response.find(' ', idStart);
    std::string idStr = response.substr(idStart, idEnd - idStart);

    uint32_t canId = std::stoul(idStr, nullptr, 16);
    uint32_t messageType = (canId >> 16) & 0xFFF;  

    return messageType;
}

float CanHandler::sendLoadRequest(uint32_t can_id) {
    sendCanMessage(can_id, nullptr, 0);
    return 0.0f;
}

float CanHandler::sendTemperatureRequest(uint32_t can_id) {
    sendCanMessage(can_id, nullptr, 0);
    return 0.0f;
}

bool CanHandler::sendRestartRequest(uint32_t can_id, const std::string& uid) {
    return sendCanMessage(can_id, reinterpret_cast<const uint8_t*>(uid.data()), uid.size());
}

bool CanHandler::sendBootloaderRequest(uint32_t can_id, const std::string& uid) {
    return sendCanMessage(can_id, reinterpret_cast<const uint8_t*>(uid.data()), uid.size());
}

bool CanHandler::sendHeaterTemperatureRequest(uint32_t can_id, float temperature) {
    return sendCanMessage(can_id, reinterpret_cast<const uint8_t*>(&temperature), sizeof(temperature));
}

float CanHandler::sendGetHeaterTemperatureRequest(uint32_t can_id) {
    sendCanMessage(can_id, nullptr, 0);
    return 0.0f;
}

bool CanHandler::sendDisableHeaterRequest(uint32_t can_id) {
    return sendCanMessage(can_id, nullptr, 0);
}

float CanHandler::sendTopSensorTemperatureRequest(uint32_t can_id) {
    sendCanMessage(can_id, nullptr, 0);
    return 0.0f;
}

float CanHandler::sendBottomSensorTemperatureRequest(uint32_t can_id) {
    sendCanMessage(can_id, nullptr, 0);
    return 0.0f;
}

bool CanHandler::sendSupplyTypeRequest(uint32_t can_id) {
    return sendCanMessage(can_id, nullptr, 0);
}

std::vector<std::string> CanHandler::sendAvailableModulesRequest(uint32_t can_id) {
    sendCanMessage(can_id, nullptr, 0);
    return {};
}

float CanHandler::sendSystemTemperatureRequest(uint32_t can_id) {
    sendCanMessage(can_id, nullptr, 0);
    return 0.0f;
}
