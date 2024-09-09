#pragma once

#include "CanBus.hpp"
#include <boost/asio.hpp>
#include <functional>
#include <memory>
#include <vector>

/**
 * @enum CanRequestStatus
 * @brief Enum representing the status of a CAN request.
 */
enum class CanRequestStatus { Success, Timeout, Fail };

/**
 * @class CanRequest
 * @brief Class representing a CAN bus request.
 * 
 * CanRequest handles the sending of CAN bus requests and the receiving of responses,
 * including support for multiple responses.
 */
class CanRequest {
public:
    /**
     * @brief Constructor for CanRequest.
     * 
     * @param canBus Reference to the CanBus object.
     * @param io_context Reference to the Boost ASIO io_context.
     * @param requestId CAN ID of the request.
     * @param data Data to send in the request.
     * @param responseId Expected CAN ID of the response.
     * @param timeoutSeconds Timeout in seconds for the request.
     * @param compareFullId Flag indicating whether to compare the full CAN ID.
     */
    CanRequest(CanBus& canBus, boost::asio::io_context& io_context, uint32_t requestId, const std::vector<uint8_t>& data, uint32_t responseId, int timeoutSeconds, bool compareFullId = true);

    /**
     * @brief Initialize the CAN request object.
     * 
     * @param canBus Reference to the CanBus object.
     * @param io_context Reference to the Boost ASIO io_context.
     * @param requestId CAN ID of the request.
     * @param data Data to send in the request.
     * @param responseId Expected CAN ID of the response.
     * @param timeoutSeconds Timeout in seconds for the request.
     * @param compareFullId Flag indicating whether to compare the full CAN ID.
     */
    void initialize(CanBus& canBus, boost::asio::io_context& io_context, uint32_t requestId, const std::vector<uint8_t>& data, uint32_t responseId, int timeoutSeconds, bool compareFullId = true);

    /**
     * @brief Send the CAN request and wait for a single response.
     * 
     * @param responseHandler Function to handle the response.
     */
    void send(std::function<void(CanRequestStatus, const CanMessage&)> responseHandler);

    /**
     * @brief Send the CAN request and wait for multiple responses.
     * 
     * @param multiResponseHandler Function to handle multiple responses.
     */
    void sendMultiResponse(std::function<void(CanRequestStatus, const std::vector<CanMessage>&)> multiResponseHandler);

    /**
     * @brief Handle the received CAN response message.
     * 
     * @param message The received CAN message.
     */
    void handleResponse(const CanMessage& message);

    /**
     * @brief Check if the response matches the expected CAN ID.
     * 
     * @param responseId CAN ID of the received response.
     * @return true if the response matches the expected ID, false otherwise.
     */
    bool matchesResponse(uint32_t responseId) const;

    /**
     * @brief Check if the response matches by message type.
     * 
     * @param responseId CAN ID of the received response.
     * @return true if the response matches the expected message type, false otherwise.
     */
    bool matchesResponseByMessageType(uint32_t responseId) const;

    /**
     * @brief Reset the CanRequest object.
     * 
     * Clears the internal handlers and stored responses to prepare the request object
     * for reuse.
     */
    void reset();

private:
    /**
     * @brief Handle the timeout event for the CAN request.
     */
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

