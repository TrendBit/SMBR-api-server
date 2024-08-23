#include "CanService.hpp"

CanService::CanService()
    : canBus(), canRequest(canBus) {}

float CanService::ping(uint32_t can_id, uint8_t seq_number) {
    auto [success, responseTime] = canRequest.sendPing(can_id, seq_number);
    return success ? responseTime : -1.0f;
}
