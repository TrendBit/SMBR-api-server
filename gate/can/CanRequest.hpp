/**
 * @file CanRequest.hpp
 * @author Vojtěch Mucha
 * @version 0.1
 * @date 23.08.2024
 */

#pragma once

#include "CanBus.hpp"
#include <utility>
#include <string>

/**
 * @class CanRequest
 * @brief Handles the creation and sending of CAN requests over the CAN bus.
 */
class CanRequest {
public:
    /**
     * @brief Constructs a CanRequest object with the specified CanBus interface.
     * @param canBus A reference to the CanBus object used for communication.
     */
    CanRequest(CanBus& canBus);

    /**
     * @brief Sends a CAN message over the CAN bus and waits for a response.
     * @param can_id The identifier of the CAN message.
     * @param data The payload data of the CAN message.
     * @return A pair containing a success flag and the response time in milliseconds.
     */
    std::pair<bool, float> sendMessage(uint32_t can_id, const std::vector<uint8_t>& data);

private:
    /**
     * @brief Sends a CAN request message over the CAN bus.
     * @param request The CanMessage object containing the request.
     * @return True if the request was successfully sent, false otherwise.
     */
    bool sendRequest(const CanMessage& request);

    /**
     * @brief Receives a CAN response message from the CAN bus.
     * @param response The CanMessage object to store the received response.
     * @return True if a response was successfully received, false otherwise.
     */
    bool receiveResponse(CanMessage& response);

    CanBus& canBus; ///< Reference to the CanBus interface used for communication.
};

