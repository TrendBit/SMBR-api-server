#include "CanRequestManager.hpp"
#include <iostream>

CanRequestManager::CanRequestManager(boost::asio::io_context& io_context, CanBus& canBus)
    : io_context_(io_context), canBus_(canBus) {
    
    canBus_.asyncReceive([this](bool success, const CanMessage& message) {
        if (success) {
            handleIncomingMessage(message);
        }

        this->startReceiving();
    });
}

void CanRequestManager::startReceiving() {

    canBus_.asyncReceive([this](bool success, const CanMessage& message) {
        if (success) {
            handleIncomingMessage(message);
        }
        this->startReceiving();
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
    request->reset();
    recycledRequests_.push_back(request);
}




void CanRequestManager::addRequestWithSeq(uint32_t requestId, const std::vector<uint8_t>& data, uint32_t responseId, uint8_t seq_num, std::function<void(CanRequestStatus, const CanMessage&)> responseHandler, double timeoutSeconds) {
    
    auto request = acquireRequest();
    request->initialize(canBus_, io_context_, requestId, data, responseId, timeoutSeconds, true); 

    uint64_t requestKey = (static_cast<uint64_t>(responseId) << 8) | seq_num;
    activeRequests_[requestKey].push(request);

    request->send([this, requestKey, responseHandler, request](CanRequestStatus status, const CanMessage& response) {

        responseHandler(status, response);

        auto& queue = activeRequests_[requestKey];
        if (!queue.empty() && queue.front() == request) {
            queue.pop();
            if (queue.empty()) {
                activeRequests_.erase(requestKey);
            }
        }
        releaseRequest(request);
    });
}





void CanRequestManager::handleIncomingMessage(const CanMessage& message) {
    uint32_t receivedId = message.getId();

    for (auto& pair : activeRequests_) {
        auto& queue = pair.second;
        if (!queue.empty()) {
            auto request = queue.front();
            if (request->matchesResponseByMessageType(receivedId)) {
                if ((receivedId & PING_RESPONSE_MASK) == (static_cast<uint32_t>(Codes::Message_type::Ping_response) << 16)) {
                    handlePingMessage(message); 
                    return;
                } else {
                    request->handleResponse(message);
                    return;
                }
            }
        }
    }

    auto it = activeRequests_.find(receivedId);
    if (it != activeRequests_.end() && !it->second.empty()) {
        auto request = it->second.front();
        request->handleResponse(message);
    }
}

void CanRequestManager::handlePingMessage(const CanMessage& message) {
    uint32_t receivedId = message.getId();
    uint8_t receivedSeqNum = message.getData()[0]; 
    uint64_t requestKey = (static_cast<uint64_t>(receivedId) << 8) | receivedSeqNum;

    auto it = activeRequests_.find(requestKey);
    if (it != activeRequests_.end() && !it->second.empty()) {
        auto request = it->second.front();
        request->handleResponse(message);
    }
}
