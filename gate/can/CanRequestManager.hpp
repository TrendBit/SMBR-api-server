/**
 * @file CanRequestManager.hpp
 * @author Vojtěch Mucha
 * @version 0.1
 * @date 23.08.2024
 */

#pragma once

#include "CanBus.hpp"
#include "CanRequest.hpp"

/**
 * @class CanRequestManager
 * @brief Singleton class that manages CAN requests and communication.
 */
class CanRequestManager {
public:
    /**
     * @brief Provides access to the singleton instance of CanRequestManager.
     * @return Reference to the CanRequestManager instance.
     */
    static CanRequestManager& getInstance();

    /**
     * @brief Sends a ping request to a CAN device and measures the response time.
     * @param can_id The identifier of the CAN request.
     * @param seq_number The sequence number for the ping request.
     * @return The response time in milliseconds, or -1.0f if the request failed.
     */
    float ping(uint32_t can_id, uint8_t seq_number);

private:
    /**
     * @brief Private constructor to prevent direct instantiation.
     */
    CanRequestManager();

    CanBus* canBus; ///< Pointer to the CanBus interface used for communication.
    CanRequest* canRequest; ///< Pointer to the CanRequest object used for creating and sending requests.
};

