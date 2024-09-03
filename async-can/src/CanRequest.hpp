#pragma once

#include "CanBus.hpp"
#include <boost/asio.hpp>
#include <functional>
#include <memory>
#include <vector>

enum class CanRequestStatus { Success, Timeout, Fail };

class CanRequest {
public:
    CanRequest(CanBus& canBus, boost::asio::io_context& io_context, uint32_t requestId, const std::vector<uint8_t>& data, uint32_t responseId, int timeoutSeconds, bool compareFullId = true);
    void initialize(CanBus& canBus, boost::asio::io_context& io_context, uint32_t requestId, const std::vector<uint8_t>& data, uint32_t responseId, int timeoutSeconds, bool compareFullId = true);

    void send(std::function<void(CanRequestStatus, const CanMessage&)> responseHandler);
    void sendMultiResponse(std::function<void(CanRequestStatus, const std::vector<CanMessage>&)> multiResponseHandler);

    void handleResponse(const CanMessage& message);

    bool matchesResponse(uint32_t responseId) const; 
    bool matchesResponseByMessageType(uint32_t responseId) const; 

private:
    void onTimeout();

    CanBus* canBus_;
    CanMessage requestMessage_;
    uint32_t expectedResponseId_;
    int timeoutSeconds_;
    bool compareFullId_;
    std::shared_ptr<std::vector<CanMessage>> responses_;
    boost::asio::steady_timer timeoutTimer_;
    std::function<void(CanRequestStatus, const CanMessage&)> responseHandler_;
    std::function<void(CanRequestStatus, const std::vector<CanMessage>&)> multiResponseHandler_;
};
