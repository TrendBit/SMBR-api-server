#include "CanRequestManager.hpp"
#include <thread>

CanRequestManager::CanRequestManager() {
    canBus = new CanBus();
    canRequest = new CanRequest(*canBus);
}

CanRequestManager& CanRequestManager::getInstance() {
    static CanRequestManager instance;
    return instance;
}

// Synchronous method
std::pair<bool, std::vector<uint8_t>> CanRequestManager::sendMessage(uint32_t can_id, const std::vector<uint8_t>& data) {
    return canRequest->sendMessage(can_id, data);
}

// Asynchronous method
std::future<std::pair<bool, std::vector<uint8_t>>> CanRequestManager::sendMessageAsync(uint32_t can_id, const std::vector<uint8_t>& data) {
    return std::async(std::launch::async, &CanRequestManager::sendMessage, this, can_id, data);
}
