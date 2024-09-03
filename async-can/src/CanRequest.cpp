#include "CanRequest.hpp"
#include <iostream>

CanRequest::CanRequest(CanBus& canBus, boost::asio::io_context& io_context, uint32_t requestId, const std::vector<uint8_t>& data, uint32_t responseId, int timeoutSeconds, bool compareFullId)
    : canBus_(&canBus), requestMessage_(requestId, data), expectedResponseId_(responseId), timeoutTimer_(io_context), timeoutSeconds_(timeoutSeconds), compareFullId_(compareFullId), responses_(std::make_shared<std::vector<CanMessage>>()) {
}

void CanRequest::initialize(CanBus& canBus, boost::asio::io_context& io_context, uint32_t requestId, const std::vector<uint8_t>& data, uint32_t responseId, int timeoutSeconds, bool compareFullId) {
    canBus_ = &canBus;  
    requestMessage_ = CanMessage(requestId, data);
    expectedResponseId_ = responseId;
    timeoutSeconds_ = timeoutSeconds;
    compareFullId_ = compareFullId;
    responses_->clear();
    timeoutTimer_ = boost::asio::steady_timer(io_context);
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

    timeoutTimer_.expires_after(std::chrono::seconds(timeoutSeconds_));
    timeoutTimer_.async_wait([this](const boost::system::error_code& ec) {
        if (!ec) {
            onTimeout();
        }
    });

    canBus_->asyncSend(requestMessage_, [this](bool success) {
        if (success) {
            std::cout << "Request sent with ID: 0x" << std::hex << requestMessage_.getId()
                      << ", waiting for response with ID: 0x" << expectedResponseId_ << std::endl;
        } else {
            responseHandler_(CanRequestStatus::Fail, CanMessage(0, {}));
        }
    });
}

void CanRequest::sendMultiResponse(std::function<void(CanRequestStatus, const std::vector<CanMessage>&)> multiResponseHandler) {
    multiResponseHandler_ = multiResponseHandler;

    timeoutTimer_.expires_after(std::chrono::seconds(timeoutSeconds_));
    timeoutTimer_.async_wait([this](const boost::system::error_code& ec) {
        if (!ec) {
            onTimeout();
        }
    });

    canBus_->asyncSend(requestMessage_, [this](bool success) {
        if (success) {
            std::cout << "Request sent with ID: 0x" << std::hex << requestMessage_.getId()
                      << ", waiting for responses with ID: 0x" << expectedResponseId_ << std::endl;
        } else {
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
    } else {
        std::cerr << "Unexpected response received with ID: 0x" << std::hex << message.getId() << std::endl;
    }
}

void CanRequest::onTimeout() {
    if (multiResponseHandler_) {
        if (!responses_->empty()) {
            multiResponseHandler_(CanRequestStatus::Success, *responses_);
        } else {
            std::cerr << "Timeout occurred with no responses for request ID: 0x" << std::hex << requestMessage_.getId() << std::endl;
            multiResponseHandler_(CanRequestStatus::Timeout, {});
        }
    } else if (responseHandler_) {
        std::cerr << "Timeout occurred for request ID: 0x" << std::hex << requestMessage_.getId() << std::endl;
        responseHandler_(CanRequestStatus::Timeout, CanMessage(0, {}));
    }
}
