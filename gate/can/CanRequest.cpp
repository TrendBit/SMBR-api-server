#include "CanRequest.hpp"
#include <chrono>

CanRequest::CanRequest(CanBus& canBus)
    : canBus(canBus) {}

std::pair<bool, float> CanRequest::sendPing(uint32_t can_id, uint8_t seq_number) {
    CanMessage request(can_id, { seq_number });
    auto start_time = std::chrono::steady_clock::now();

    if (!sendRequest(request)) {
        return {false, -1.0f};
    }

    CanMessage response(0, {});
    if (!receiveResponse(response)) {
        return {false, -1.0f};
    }

    auto end_time = std::chrono::steady_clock::now();
    float elapsed = std::chrono::duration<float, std::milli>(end_time - start_time).count();

    return {true, elapsed};
}

bool CanRequest::sendRequest(const CanMessage& request) {
    return canBus.send(request);
}

bool CanRequest::receiveResponse(CanMessage& response) {
    return canBus.receive(response);
}
