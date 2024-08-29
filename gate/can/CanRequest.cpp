#include "CanRequest.hpp"

CanRequest::CanRequest(CanBus& canBus) : canBus(canBus) {}

std::pair<bool, std::vector<uint8_t>> CanRequest::sendMessage(uint32_t can_id, const std::vector<uint8_t>& data) {
    CanMessage request(can_id, data);

    if (!sendRequest(request)) {
        return {false, {}};
    }

    CanMessage response(0, {});
    if (receiveResponse(response)) {
        return {true, response.getData()};
    }

    return {false, {}};
}

bool CanRequest::sendRequest(const CanMessage& request) {
    return canBus.send(request);
}

bool CanRequest::receiveResponse(CanMessage& response) {
    return canBus.receive(response);
}
