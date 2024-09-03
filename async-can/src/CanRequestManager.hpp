#pragma once
#include "CanBus.hpp"
#include "CanRequest.hpp"
#include <boost/asio.hpp>
#include <unordered_map>
#include <queue>
#include <memory>

class CanRequestManager {
public:
    CanRequestManager(boost::asio::io_context& io_context, CanBus& canBus);

    void addRequest(uint32_t requestId, const std::vector<uint8_t>& data, uint32_t responseId, std::function<void(CanRequestStatus, const CanMessage&)> responseHandler);

    void handleIncomingMessage(const CanMessage& message);

private:
    boost::asio::io_context& io_context_;
    CanBus& canBus_;
    std::unordered_map<uint32_t, std::queue<std::shared_ptr<CanRequest>>> activeRequests_;
};
