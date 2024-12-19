#include "CanRequestManager.hpp"
#include <spdlog/spdlog.h>

CanRequestManager::CanRequestManager(boost::asio::io_context& io_context, CanBus& canBus)
    : io_context_(io_context), canBus_(canBus) {

    canBus_.asyncReceive([this](bool success, const CanMessage& message) {
        if (success) {
            handleIncomingMessage(message);
        } else {
            spdlog::warn("Message reception failed in constructor");
        }
        this->startReceiving();
    });
}

void CanRequestManager::startReceiving() {
    canBus_.asyncReceive([this](bool success, const CanMessage& message) {
        if (success) {
            handleIncomingMessage(message);
        } else {
            spdlog::warn("Message reception failed in startReceiving");
        }
        this->startReceiving();
    });
}

std::unique_ptr<CanRequest> CanRequestManager::acquireRequest() {
    std::lock_guard<std::mutex> lock(recycledRequestsMutex_);

    if (!recycledRequests_.empty()) {
        auto request = std::move(recycledRequests_.back());
        recycledRequests_.pop_back();
        return request;
    } else {
        return std::make_unique<CanRequest>(canBus_, io_context_, 0, std::vector<uint8_t>(), 0, 0);
    }
}

void CanRequestManager::releaseRequest(std::unique_ptr<CanRequest> request) {
    {
        std::lock_guard<std::mutex> activeLock(activeRequestsMutex_);
        for (const auto& [key, queue] : activeRequests_) {
            if (!queue.empty() && queue.front().get() == request.get()) {
                spdlog::warn("Attempted to recycle an active request");
                return;
            }
        }
    }

    std::lock_guard<std::mutex> recycledLock(recycledRequestsMutex_);
    request->reset();
    recycledRequests_.push_back(std::move(request));
}



void CanRequestManager::addRequestWithSeq(uint32_t requestId, const std::vector<uint8_t>& data, uint32_t responseId, uint8_t seq_num, std::function<void(CanRequestStatus, const CanMessage&)> responseHandler, double timeoutSeconds) {
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
            releaseRequest(std::move(completedRequest));
        }
    });
}



void CanRequestManager::handleIncomingMessage(const CanMessage& message) {
    uint32_t receivedId = message.getId();

    for (auto& [key, queue] : activeRequests_) {
        if (!queue.empty() && queue.front() && queue.front()->matchesResponseByMessageType(receivedId)) {
            queue.front()->handleResponse(message);
            return;
        }
    }

    spdlog::warn("No matching request found for message ID={}", receivedId);
}

/*
void CanRequestManager::handleIncomingMessage(const CanMessage& message) {
    uint32_t receivedId = message.getId();

    std::unique_ptr<CanRequest> requestToHandle;
    {
        // Zamykáme pouze přístup k activeRequests_
        std::lock_guard<std::mutex> lock(activeRequestsMutex_);
        for (auto& [key, queue] : activeRequests_) {
            if (!queue.empty() && queue.front() && queue.front()->matchesResponseByMessageType(receivedId)) {
                requestToHandle = std::move(queue.front());
                queue.pop();
                if (queue.empty()) {
                    activeRequests_.erase(key);
                }
                break;
            }
        }
    }

    if (requestToHandle) {
        // Zpracování požadavku mimo zamknutou sekci
        requestToHandle->handleResponse(message);
    } else {
        spdlog::warn("No matching request found for message ID={}", receivedId);
    }
}*/


void CanRequestManager::handlePingMessage(const CanMessage& message) {
    uint32_t receivedId = message.getId();
    uint8_t receivedSeqNum = message.getData()[0];
    uint64_t requestKey = (static_cast<uint64_t>(receivedId) << 8) | receivedSeqNum;

    auto it = activeRequests_.find(requestKey);
    if (it != activeRequests_.end() && !it->second.empty()) {
        auto& request = it->second.front();
        if (request) {
            request->handleResponse(message);
        }
    } else {
        spdlog::warn("No matching ping request found for key={}", requestKey);
    }
}