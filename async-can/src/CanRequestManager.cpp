#include "CanRequestManager.hpp"
#include <iostream>

CanRequestManager::CanRequestManager(boost::asio::io_context& io_context, CanBus& canBus)
    : io_context_(io_context), canBus_(canBus) {
    std::cout << "CanRequestManager initialized" << std::endl;

    canBus_.asyncReceive([this](bool success, const CanMessage& message) {
        if (success) {
            handleIncomingMessage(message);
        } else {
            std::cerr << "Failed to receive CAN message." << std::endl;
        }
        canBus_.asyncReceive([this](bool success, const CanMessage& message) {
            if (success) {
                handleIncomingMessage(message);
            }
        });
    });
}

void CanRequestManager::addRequest(uint32_t requestId, const std::vector<uint8_t>& data, uint32_t responseId, std::function<void(CanRequestStatus, const CanMessage&)> responseHandler) {
    auto request = std::make_shared<CanRequest>(canBus_, io_context_, requestId, data, responseId);

    activeRequests_[responseId].push(request);

    request->send([this, responseId, responseHandler, request](CanRequestStatus status, const CanMessage& response) {  
        responseHandler(status, response);

        auto& queue = activeRequests_[responseId];
        if (!queue.empty() && queue.front() == request) {
            queue.pop();
            if (queue.empty()) {
                activeRequests_.erase(responseId); 
            }
        }
    });
}

void CanRequestManager::handleIncomingMessage(const CanMessage& message) {
    auto it = activeRequests_.find(message.getId());
    if (it != activeRequests_.end() && !it->second.empty()) {
        auto request = it->second.front();  
        request->handleResponse(message);   
    }
}
