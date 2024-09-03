#include "CanRequest.hpp"
#include <iostream>

CanRequest::CanRequest(CanBus& canBus, boost::asio::io_context& io_context, uint32_t requestId, const std::vector<uint8_t>& data, uint32_t responseId)
    : canBus_(canBus), requestMessage_(requestId, data), expectedResponseId_(responseId), timeoutTimer_(io_context) {}

void CanRequest::send(std::function<void(CanRequestStatus, const CanMessage&)> responseHandler) {
    responseHandler_ = responseHandler;

    // Start timeout timer for 8 seconds
    timeoutTimer_.expires_after(std::chrono::seconds(8));
    timeoutTimer_.async_wait([this](const boost::system::error_code& ec) {
        if (!ec) {
            onTimeout();
        }
    });

    canBus_.asyncSend(requestMessage_, [this](bool success) {
        if (success) {
            std::cout << "Request sent with ID: 0x" << std::hex << requestMessage_.getId() 
                      << ", waiting for response with ID: 0x" << expectedResponseId_ << std::endl;
        } else {
            responseHandler_(CanRequestStatus::Fail, CanMessage(0, {}));
        }
    });
}

bool CanRequest::matchesResponse(uint32_t responseId) const {
    return responseId == expectedResponseId_;
}

void CanRequest::handleResponse(const CanMessage& message) {
    timeoutTimer_.cancel(); 
    responseHandler_(CanRequestStatus::Success, message);
}

void CanRequest::onTimeout() {
    std::cerr << "Timeout occurred for request ID: 0x" << std::hex << requestMessage_.getId() << std::endl;
    responseHandler_(CanRequestStatus::Timeout, CanMessage(0, {}));
}
