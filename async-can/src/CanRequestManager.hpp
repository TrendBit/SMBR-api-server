#pragma once

#include "CanRequest.hpp"
#include <boost/asio.hpp>
#include <unordered_map>
#include <queue>
#include <memory>
#include <vector>

class CanRequestManager {
public:
    CanRequestManager(boost::asio::io_context& io_context, CanBus& canBus);

    void addRequest(uint32_t requestId, const std::vector<uint8_t>& data, uint32_t responseId, std::function<void(CanRequestStatus, const CanMessage&)> responseHandler, int timeoutSeconds);
    void addMultiResponseRequest(uint32_t requestId, const std::vector<uint8_t>& data, uint32_t responseId, std::function<void(CanRequestStatus, const std::vector<CanMessage>&)> multiResponseHandler, int timeoutSeconds);

private:
    void handleIncomingMessage(const CanMessage& message);
    std::shared_ptr<CanRequest> acquireRequest();
    void releaseRequest(std::shared_ptr<CanRequest> request);

    boost::asio::io_context& io_context_;
    CanBus& canBus_;
    std::unordered_map<uint32_t, std::queue<std::shared_ptr<CanRequest>>> activeRequests_;
    std::vector<std::shared_ptr<CanRequest>> recycledRequests_;
};
