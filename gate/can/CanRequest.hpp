#ifndef CANREQUEST_HPP
#define CANREQUEST_HPP

#include "CanBus.hpp"
#include <utility>
#include <string>

class CanRequest {
public:
    CanRequest(CanBus& canBus);

    std::pair<bool, float> sendPing(uint32_t can_id, uint8_t seq_number);

private:
    CanBus& canBus;
    bool sendRequest(const CanMessage& request);
    bool receiveResponse(CanMessage& response);
};

#endif // CANREQUEST_HPP
