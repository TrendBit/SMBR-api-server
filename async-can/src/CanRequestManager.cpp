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

std::shared_ptr<CanRequest> CanRequestManager::acquireRequest() {
    if (!recycledRequests_.empty()) {
        auto request = recycledRequests_.back();
        recycledRequests_.pop_back();
        return request;
    } else {
        return std::make_shared<CanRequest>(canBus_, io_context_, 0, std::vector<uint8_t>(), 0, 0);
    }
}

void CanRequestManager::releaseRequest(std::shared_ptr<CanRequest> request) {
    recycledRequests_.push_back(request);
}

void CanRequestManager::addRequest(uint32_t requestId, const std::vector<uint8_t>& data, uint32_t responseId, std::function<void(CanRequestStatus, const CanMessage&)> responseHandler, int timeoutSeconds) {
    auto request = acquireRequest();
    request->initialize(canBus_, io_context_, requestId, data, responseId, timeoutSeconds);

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

        releaseRequest(request);
    });
}

void CanRequestManager::addMultiResponseRequest(uint32_t requestId, const std::vector<uint8_t>& data, uint32_t responseId, std::function<void(CanRequestStatus, const std::vector<CanMessage>&)> multiResponseHandler, int timeoutSeconds) {
    auto request = acquireRequest();
    request->initialize(canBus_, io_context_, requestId, data, responseId, timeoutSeconds);

    activeRequests_[responseId].push(request);

    request->sendMultiResponse([this, responseId, multiResponseHandler, request](CanRequestStatus status, const std::vector<CanMessage>& responses) {
        multiResponseHandler(status, responses);

        auto& queue = activeRequests_[responseId];
        if (!queue.empty() && queue.front() == request) {
            queue.pop();
            if (queue.empty()) {
                activeRequests_.erase(responseId);
            }
        }

        releaseRequest(request);
    });
}

void CanRequestManager::handleIncomingMessage(const CanMessage& message) {
    uint32_t maskedId = message.getId() & 0x0FFF0000;

    for (auto& pair : activeRequests_) {
        uint32_t expectedMaskedId = pair.first & 0x0FFF0000;

        if (maskedId == expectedMaskedId) {
            if (!pair.second.empty()) {
                auto request = pair.second.front();
                request->handleResponse(message);
                return;
            }
        }
    }

    std::cerr << "Unexpected response received with ID: 0x" << std::hex << message.getId() << std::endl;
}
