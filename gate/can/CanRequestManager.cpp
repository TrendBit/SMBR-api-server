#include "CanRequestManager.hpp"

CanRequestManager::CanRequestManager() {
    canBus = new CanBus();  
    canRequest = new CanRequest(*canBus);
}

CanRequestManager& CanRequestManager::getInstance() {
    static CanRequestManager instance;
    return instance;
}

float CanRequestManager::sendMessage(uint32_t can_id, const std::vector<uint8_t>& data) {
    auto [success, response_time] = canRequest->sendMessage(can_id, data);
    return success ? response_time : -1.0f;
}
