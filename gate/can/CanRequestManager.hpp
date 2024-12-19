#pragma once

#include "CanRequest.hpp"
#include <boost/asio.hpp>
#include <unordered_map>
#include <queue>
#include <memory>
#include <vector>
#include "codes/codes.hpp"
#include <mutex>

class CanRequestManager {
public:
    static constexpr uint32_t PING_RESPONSE_MASK = 0xFFFF0000;

    CanRequestManager(boost::asio::io_context& io_context, CanBus& canBus);

    void addRequestWithSeq(uint32_t requestId, const std::vector<uint8_t>& data, uint32_t responseId, uint8_t seq_num, std::function<void(CanRequestStatus, const CanMessage&)> responseHandler, double timeoutSeconds);

private:
    void startReceiving();
    void handleIncomingMessage(const CanMessage& message);
    void handlePingMessage(const CanMessage& message);

    std::mutex activeRequestsMutex_;
    std::mutex recycledRequestsMutex_;

    std::unique_ptr<CanRequest> acquireRequest();
    void releaseRequest(std::unique_ptr<CanRequest> request);

    boost::asio::io_context& io_context_;
    CanBus& canBus_;
    std::unordered_map<uint32_t, std::queue<std::unique_ptr<CanRequest>>> activeRequests_;
    std::vector<std::unique_ptr<CanRequest>> recycledRequests_;
};