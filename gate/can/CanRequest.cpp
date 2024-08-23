#include "CanRequest.hpp"
#include <chrono>

CanRequest::CanRequest(CanBus& canBus) : canBus(canBus) {}

std::pair<bool, float> CanRequest::sendMessage(uint32_t can_id, const std::vector<uint8_t>& data) {
    CanMessage request(can_id, data);
    auto start_time = std::chrono::steady_clock::now();

    if (!sendRequest(request)) {
        return {false, -1.0f};
    }

    CanMessage response(0, {});  
    if (receiveResponse(response)) {
        auto response_time = std::chrono::duration<float, std::milli>(std::chrono::steady_clock::now() - start_time).count();
        return {true, response_time};
    }

    return {false, -1.0f};
}

bool CanRequest::sendRequest(const CanMessage& request) {
    return canBus.send(request);
}

bool CanRequest::receiveResponse(CanMessage& response) {
    return canBus.receive(response);
}
