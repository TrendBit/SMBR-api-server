#include "CanRequestManager.hpp"

CanRequestManager::CanRequestManager() {
    canBus = new CanBus();  
    canRequest = new CanRequest(*canBus);
}

CanRequestManager& CanRequestManager::getInstance() {
    static CanRequestManager instance;
    return instance;
}

float CanRequestManager::ping(uint32_t can_id, uint8_t seq_number) {
    auto [success, responseTime] = canRequest->sendPing(can_id, seq_number);
    return success ? responseTime : -1.0f;
}
