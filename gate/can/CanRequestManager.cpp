#include "CanRequestManager.hpp"
#include <spdlog/spdlog.h>

extern backward::SignalHandling sh;

CanRequestManager::CanRequestManager(boost::asio::io_context& io_context, CanBus& canBus)
    : io_context_(io_context), canBus_(canBus) {
    spdlog::info("[CanRequestManager] CanRequestManager initialized");

    canBus_.asyncReceive([this](bool success, const CanMessage& message) {
        if (success) {
            spdlog::debug("[CanRequestManager] Received message: {}", message.getId());
            handleIncomingMessage(message);
        } else {
            spdlog::warn("[CanRequestManager] Failed to receive message");
        } 
        this->startReceiving();
    });
}

void CanRequestManager::startReceiving() {
    canBus_.asyncReceive([this](bool success, const CanMessage& message) {
        if (success) {
            spdlog::debug("[CanRequestManager] Received message: {}", message.getId());
            handleIncomingMessage(message);
        } else {
            spdlog::warn("[CanRequestManager] Failed to receive message");
        } 
        this->startReceiving();
    });
}

std::unique_ptr<CanRequest> CanRequestManager::acquireRequest() {
    std::lock_guard<std::mutex> lock(recycledRequestsMutex_);

    if (!recycledRequests_.empty()) {
        auto request = std::move(recycledRequests_.back());
        recycledRequests_.pop_back();
        spdlog::debug("[CanRequestManager] Reusing request");
        return request;
    } else {
        spdlog::debug("[CanRequestManager] Creating new request");
        return std::make_unique<CanRequest>(canBus_, io_context_, 0, std::vector<uint8_t>(), 0, 0);
    }
}

void CanRequestManager::releaseRequest(std::unique_ptr<CanRequest> request) {
    {
        std::lock_guard<std::mutex> activeLock(activeRequestsMutex_);
        for (const auto& [key, queue] : activeRequests_) {
            if (!queue.empty() && queue.front().get() == request.get()) {
                spdlog::debug("[CanRequestManager] Request already active, not releasing");
                return;
            }
        }
    }

    std::lock_guard<std::mutex> recycledLock(recycledRequestsMutex_);
    request->reset();
    recycledRequests_.push_back(std::move(request));
    spdlog::debug("[CanRequestManager] Request released back to recycled pool");
}

void CanRequestManager::addRequest(uint32_t requestId, const std::vector<uint8_t>& data, uint32_t responseId, std::function<void(CanRequestStatus, const CanMessage&)> responseHandler, double timeoutSeconds) {
    spdlog::info("[CanRequestManager] Adding request: requestId = {}, responseId = {}", requestId, responseId);
    
    auto request = acquireRequest();
    request->initialize(canBus_, io_context_, requestId, data, responseId, timeoutSeconds, true);

    {
        std::lock_guard<std::mutex> lock(activeRequestsMutex_);
        activeRequests_[responseId].push(std::move(request));
        spdlog::info("[CanRequestManager] Queue size for responseId 0x{:X}: {}", responseId, activeRequests_[responseId].size());
    }

    auto& queue = activeRequests_[responseId];
    spdlog::info("[CanRequestManager] Current queue size for responseId 0x{:X}: {}", responseId, queue.size());
    queue.front()->send([this, responseId, responseHandler](CanRequestStatus status, const CanMessage& response) mutable {
        responseHandler(status, response);

        std::unique_ptr<CanRequest> completedRequest;
        {
            std::lock_guard<std::mutex> lock(activeRequestsMutex_);
            auto it = activeRequests_.find(responseId);
            if (it != activeRequests_.end() && !it->second.empty()) {
                completedRequest = std::move(it->second.front());
                it->second.pop();
                if (it->second.empty()) {
                    activeRequests_.erase(it);
                }
            }
        }
        spdlog::info("[CanRequestManager] Request with responseId 0x{:X} processed, queue size: {}", responseId, activeRequests_[responseId].size());

        if (completedRequest) {
            spdlog::debug("[CanRequestManager] Request completed and released");
            releaseRequest(std::move(completedRequest));
        }
    });
}

void CanRequestManager::sendWithoutResponse(uint32_t requestId, const std::vector<uint8_t>& data, std::function<void(bool)> resultHandler) {
    spdlog::info("[CanRequestManager] Sending request without expecting response: requestId = {}", requestId);

    auto request = acquireRequest();
    request->initializeForSendOnly(canBus_, io_context_, requestId, data);
    request->sendOnly(resultHandler);  
}

void CanRequestManager::addRequestWithSeq(uint32_t requestId, const std::vector<uint8_t>& data, uint32_t responseId, uint8_t seq_num, std::function<void(CanRequestStatus, const CanMessage&)> responseHandler, double timeoutSeconds) {
    spdlog::info("[CanRequestManager] Adding sequential request: requestId = {}, responseId = {}, seq_num = {}", requestId, responseId, seq_num);

    auto request = acquireRequest();
    request->initialize(canBus_, io_context_, requestId, data, responseId, timeoutSeconds, true);

    uint64_t requestKey = (static_cast<uint64_t>(responseId) << 8) | seq_num;

    {
        std::lock_guard<std::mutex> lock(activeRequestsMutex_);
        activeRequests_[requestKey].push(std::move(request));
    }

    auto& queue = activeRequests_[requestKey];
    queue.front()->send([this, requestKey, responseHandler](CanRequestStatus status, const CanMessage& response) mutable {
        responseHandler(status, response);

        std::unique_ptr<CanRequest> completedRequest;
        {
            std::lock_guard<std::mutex> lock(activeRequestsMutex_);
            auto it = activeRequests_.find(requestKey);
            if (it != activeRequests_.end() && !it->second.empty()) {
                completedRequest = std::move(it->second.front());
                it->second.pop();
                if (it->second.empty()) {
                    activeRequests_.erase(it);
                }
            }
        }

        if (completedRequest) {
            spdlog::debug("[CanRequestManager] Sequential request completed and released");
            releaseRequest(std::move(completedRequest));
        }
    });
}

void CanRequestManager::addMultiResponseRequest(uint32_t requestId, const std::vector<uint8_t>& data, uint32_t responseId, std::function<void(CanRequestStatus, const std::vector<CanMessage>&)> multiResponseHandler, double timeoutSeconds) {
    spdlog::info("[CanRequestManager] Adding multi-response request: requestId = {}, responseId = {}", requestId, responseId);

    auto request = acquireRequest();
    request->initialize(canBus_, io_context_, requestId, data, responseId, timeoutSeconds, false);

    {
        std::lock_guard<std::mutex> lock(activeRequestsMutex_);
        activeRequests_[responseId].push(std::move(request));
    }

    auto& queue = activeRequests_[responseId];
    auto frontRequest = queue.front().get(); 
    frontRequest->sendMultiResponse([this, responseId, multiResponseHandler, frontRequest](CanRequestStatus status, const std::vector<CanMessage>& responses) mutable {
        if (multiResponseHandler) {
            multiResponseHandler(status, responses);
        }

        std::unique_ptr<CanRequest> completedRequest;
        {
            std::lock_guard<std::mutex> lock(activeRequestsMutex_);
            auto it = activeRequests_.find(responseId);
            if (it != activeRequests_.end() && !it->second.empty()) {
                completedRequest = std::move(it->second.front());
                it->second.pop();
                if (it->second.empty()) {
                    activeRequests_.erase(it);
                }
            }
        }

        if (completedRequest) {
            spdlog::debug("[CanRequestManager] Multi-response request completed and released");
            releaseRequest(std::move(completedRequest));
        }
    });
}

void CanRequestManager::handleIncomingMessage(const CanMessage& message) {
    uint32_t receivedId = message.getId();
    spdlog::debug("[CanRequestManager] Handling incoming message: receivedId = {}", receivedId);

    for (auto& [key, queue] : activeRequests_) {
        if (!queue.empty() && queue.front() && queue.front()->matchesResponseByMessageType(receivedId)) {
            spdlog::debug("[CanRequestManager] Matching response found for requestId = {}", receivedId);
            queue.front()->handleResponse(message);
            return;
        }
    }

    spdlog::warn("[CanRequestManager] No matching request found for receivedId = {}", receivedId);
}

void CanRequestManager::handlePingMessage(const CanMessage& message) {
    uint32_t receivedId = message.getId();
    uint8_t receivedSeqNum = message.getData()[0];
    uint64_t requestKey = (static_cast<uint64_t>(receivedId) << 8) | receivedSeqNum;

    spdlog::debug("[CanRequestManager] Handling ping message: receivedId = {}, receivedSeqNum = {}", receivedId, receivedSeqNum);

    auto it = activeRequests_.find(requestKey);
    if (it != activeRequests_.end() && !it->second.empty()) {
        auto& request = it->second.front();
        if (request) {
            spdlog::debug("[CanRequestManager] Ping response for requestKey = {} handled", requestKey);
            request->handleResponse(message);
        }
    } else {
        spdlog::warn("[CanRequestManager] No matching request found for ping with requestKey = {}", requestKey);
    } 
}
