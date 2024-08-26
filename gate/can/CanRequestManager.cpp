#include "CanRequestManager.hpp"

CanRequestManager::CanRequestManager() {
    canBus = new CanBus();
    canRequest = new CanRequest(*canBus);
}

CanRequestManager& CanRequestManager::getInstance() {
    static CanRequestManager instance;
    return instance;
}

std::pair<bool, std::vector<uint8_t>> CanRequestManager::sendMessage(uint32_t can_id, const std::vector<uint8_t>& data) {
    return canRequest->sendMessage(can_id, data);
}
