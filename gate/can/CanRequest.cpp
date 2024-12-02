#include "CanRequest.hpp"

CanRequest::CanRequest(CanBus& canBus, boost::asio::io_context& io_context, uint32_t requestId, const std::vector<uint8_t>& data, uint32_t responseId, double timeoutSeconds, bool compareFullId)
    : canBus_(&canBus), requestMessage_(requestId, data), expectedResponseId_(responseId), timeoutTimer_(io_context), timeoutSeconds_(timeoutSeconds), compareFullId_(compareFullId), responses_(std::make_shared<std::vector<CanMessage>>()) {
}

void CanRequest::initialize(CanBus& canBus, boost::asio::io_context& io_context, uint32_t requestId, const std::vector<uint8_t>& data, uint32_t responseId, double timeoutSeconds, bool compareFullId) {
    canBus_ = &canBus;  
    requestMessage_ = CanMessage(requestId, data);
    expectedResponseId_ = responseId;
    timeoutSeconds_ = timeoutSeconds * 1000; 
    compareFullId_ = compareFullId;
    responses_->clear();
    timeoutTimer_ = boost::asio::steady_timer(io_context);
}

void CanRequest::initializeForSendOnly(CanBus& canBus, boost::asio::io_context& io_context, uint32_t requestId, const std::vector<uint8_t>& data) {
    canBus_ = &canBus;  
    requestMessage_ = CanMessage(requestId, data);
}


bool CanRequest::matchesResponse(uint32_t responseId) const {
    if (compareFullId_) {
        return responseId == expectedResponseId_;
    } else {
        return matchesResponseByMessageType(responseId);
    }
}

bool CanRequest::matchesResponseByMessageType(uint32_t responseId) const {
    uint32_t responseMessageType = responseId & 0xFFFF0000;
    uint32_t expectedMessageType = expectedResponseId_ & 0xFFFF0000;
    return responseMessageType == expectedMessageType;
}

void CanRequest::send(std::function<void(CanRequestStatus, const CanMessage&)> responseHandler) {
    responseHandler_ = responseHandler;

    timeoutTimer_.expires_after(std::chrono::milliseconds(static_cast<int>(timeoutSeconds_)));
    timeoutTimer_.async_wait([this](const boost::system::error_code& ec) {
        if (!ec) {
            onTimeout();
        }
    });

    canBus_->asyncSend(requestMessage_, [this](bool success) {
        if (!success) {
            responseHandler_(CanRequestStatus::Fail, CanMessage(0, {}));
        }
    });
}

void CanRequest::sendOnly(std::function<void(bool)> resultHandler) {
    canBus_->asyncSend(requestMessage_, [resultHandler](bool success) {
        resultHandler(success);
    });
}


void CanRequest::sendMultiResponse(std::function<void(CanRequestStatus, const std::vector<CanMessage>&)> multiResponseHandler) {
    multiResponseHandler_ = multiResponseHandler;

    timeoutTimer_.expires_after(std::chrono::milliseconds(static_cast<int>(timeoutSeconds_)));
    timeoutTimer_.async_wait([this](const boost::system::error_code& ec) {
        if (!ec) {
            onTimeout();
        }
    });

    canBus_->asyncSend(requestMessage_, [this](bool success) {
        if (!success) {
            multiResponseHandler_(CanRequestStatus::Fail, {});
        }
    });
}

void CanRequest::handleResponse(const CanMessage& message) {
    if (matchesResponse(message.getId())) {
        if (multiResponseHandler_) {
            responses_->push_back(message);
        } else if (responseHandler_) {
            timeoutTimer_.cancel();
            responseHandler_(CanRequestStatus::Success, message);
        }
    }
}

void CanRequest::onTimeout() {
    if (multiResponseHandler_) {
        if (!responses_->empty()) {
            multiResponseHandler_(CanRequestStatus::Success, *responses_);
        } else {
            multiResponseHandler_(CanRequestStatus::Timeout, {});
        }
    } else if (responseHandler_) {
        responseHandler_(CanRequestStatus::Timeout, CanMessage(0, {}));
    }
}

void CanRequest::reset() {
    responseHandler_ = nullptr;
    multiResponseHandler_ = nullptr;
    responses_->clear();
}