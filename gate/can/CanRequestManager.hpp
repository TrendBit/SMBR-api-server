#pragma once

#include "CanRequest.hpp"
#include <boost/asio.hpp>
#include <unordered_map>
#include <queue>
#include <memory>
#include <vector>
#include "codes/codes.hpp"
#include <mutex>

#include "backward.hpp"

extern backward::SignalHandling sh;

/**
 * @class CanRequestManager
 * @brief Class responsible for managing CAN bus requests.
 * 
 * CanRequestManager provides functionality for sending and receiving single or multiple
 * CAN bus requests and handling responses.
 */
class CanRequestManager {
public:
    static constexpr uint32_t PING_RESPONSE_MASK = 0xFFFF0000;
    /**
     * @brief Constructor for CanRequestManager.
     * 
     * @param io_context Reference to Boost ASIO io_context for asynchronous operations.
     * @param canBus Reference to the CanBus object for handling CAN communications.
     */
    CanRequestManager(boost::asio::io_context& io_context, CanBus& canBus);

    /**
     * @brief Add a request to the CAN bus.
     * 
     * @param requestId CAN ID of the request.
     * @param data Data to send in the request.
     * @param responseId Expected CAN ID of the response.
     * @param responseHandler Function to handle the response.
     * @param timeoutSeconds Timeout in seconds for the request.
     */
    void addRequest(uint32_t requestId, const std::vector<uint8_t>& data, uint32_t responseId, std::function<void(CanRequestStatus, const CanMessage&)> responseHandler, double timeoutSeconds);

    /**
     * @brief Send a CAN request without expecting a response.
     * 
     * @param requestId CAN ID of the request.
     * @param data Data to send in the request.
     * @param resultHandler Callback function to indicate if the send operation was successful.
     */
    void sendWithoutResponse(uint32_t requestId, const std::vector<uint8_t>& data, std::function<void(bool)> resultHandler);


/**
     * @brief Add a request to the CAN bus with a sequence number for ordered responses.
     * 
     * @param requestId CAN ID of the request.
     * @param data Data to send in the request.
     * @param responseId Expected CAN ID of the response.
     * @param seq_num Sequence number to match in the response for ordered communication.
     * @param responseHandler Callback function to handle the response. Receives the status
     *                        of the request and the CAN message if successful.
     * @param timeoutSeconds Timeout in seconds for the request.
     */
    void addRequestWithSeq(uint32_t requestId, const std::vector<uint8_t>& data, uint32_t responseId, uint8_t seq_num, std::function<void(CanRequestStatus, const CanMessage&)> responseHandler, double timeoutSeconds);
    

    /**
     * @brief Add a request to the CAN bus expecting multiple responses.
     * 
     * @param requestId CAN ID of the request.
     * @param data Data to send in the request.
     * @param responseId Expected CAN ID of the responses.
     * @param multiResponseHandler Function to handle multiple responses.
     * @param timeoutSeconds Timeout in seconds for the request.
     */
    void addMultiResponseRequest(uint32_t requestId, const std::vector<uint8_t>& data, uint32_t responseId, std::function<void(CanRequestStatus, const std::vector<CanMessage>&)> multiResponseHandler, double timeoutSeconds);

private:
    /**
     * @brief Handle incoming CAN messages and match them to active requests.
     * 
     * @param message The received CAN message.
     */
    void handleIncomingMessage(const CanMessage& message);

    /**
     * @brief Handle a special type of CAN message (ping message).
     * 
     * @param message The received CAN message to handle as a ping message.
     */
    void handlePingMessage(const CanMessage& message);
   

    /**
     * @brief Acquire a CanRequest object from the recycled pool or create a new one.
     * 
     * @return std::shared_ptr<CanRequest> Pointer to a CanRequest object.
     */
    std::unique_ptr<CanRequest> acquireRequest();

    /**
     * @brief Release a CanRequest object back to the recycled pool.
     * 
     * @param request Pointer to the CanRequest object to release.
     */
    void releaseRequest(std::unique_ptr<CanRequest> request);

    void startReceiving();
    std::mutex activeRequestsMutex_;
    std::mutex recycledRequestsMutex_;

    boost::asio::io_context& io_context_;
    CanBus& canBus_;
    std::unordered_map<uint32_t, std::queue<std::unique_ptr<CanRequest>>> activeRequests_;
    std::vector<std::unique_ptr<CanRequest>> recycledRequests_;
};