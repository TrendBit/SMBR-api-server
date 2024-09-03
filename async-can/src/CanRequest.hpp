#pragma once
#include "CanBus.hpp"
#include <boost/asio.hpp>
#include <functional>
#include <vector>

enum class CanRequestStatus {
    Success,
    Fail,
    Timeout
};

class CanRequest {
public:
    CanRequest(CanBus& canBus, boost::asio::io_context& io_context, uint32_t requestId, const std::vector<uint8_t>& data, uint32_t responseId);

    void send(std::function<void(CanRequestStatus, const CanMessage&)> responseHandler);

    bool matchesResponse(uint32_t responseId) const;
    void handleResponse(const CanMessage& message);

private:
    CanBus& canBus_;
    CanMessage requestMessage_;
    uint32_t expectedResponseId_;
    std::function<void(CanRequestStatus, const CanMessage&)> responseHandler_;
    boost::asio::steady_timer timeoutTimer_;

    void onTimeout();
};
