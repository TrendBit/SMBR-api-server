/**
 * @file CanRequestManager.hpp
 * @author Vojtěch Mucha
 * @version 0.1
 * @date 28.08.2024
 */

#pragma once

#include <future>
#include "CanBus.hpp"
#include "CanRequest.hpp"
#include <utility>
#include <vector>

/**
 * @class CanRequestManager
 * @brief Singleton class for managing and processing CAN requests.
 *
 * The CanRequestManager class serves as the main interface for sending requests and receiving responses over the CAN bus.
 * It provides both synchronous and asynchronous methods for sending messages via CAN.
 */
class CanRequestManager {
public:
    /**
     * @brief Retrieves the singleton instance of CanRequestManager.
     * 
     * This method returns the single instance of the CanRequestManager class, ensuring that only one instance exists
     * throughout the application.
     * 
     * @return Reference to the CanRequestManager instance.
     */
    static CanRequestManager& getInstance();

    /**
     * @brief Asynchronously sends a message over the CAN bus.
     * 
     * This method sends a message over the CAN bus asynchronously and returns a `std::future` that allows checking the status
     * of the operation and retrieving the result once it becomes available.
     * 
     * @param can_id CAN identifier of the message.
     * @param data Data to be sent as the content of the CAN message.
     * @return `std::future` containing a pair of `bool` and `std::vector<uint8_t>`. The `bool` element indicates the success
     * of the operation, and `std::vector<uint8_t>` contains the response data.
     */
    std::future<std::pair<bool, std::vector<uint8_t>>> sendMessageAsync(uint32_t can_id, const std::vector<uint8_t>& data);

private:
    /**
     * @brief Private constructor for CanRequestManager.
     * 
     * The constructor is private to prevent the creation of multiple instances of the class. It is used only within `getInstance`.
     */
    CanRequestManager();

    /**
     * @brief Synchronously sends a message over the CAN bus.
     * 
     * This method sends a message over the CAN bus and waits for a response. The operation is blocking, meaning that the method
     * will not return until the response is received or the timeout expires.
     * 
     * @param can_id CAN identifier of the message.
     * @param data Data to be sent as the content of the CAN message.
     * @return A pair of `bool` and `std::vector<uint8_t>`. The `bool` element indicates the success of the operation,
     * and `std::vector<uint8_t>` contains the response data.
     */
    std::pair<bool, std::vector<uint8_t>> sendMessage(uint32_t can_id, const std::vector<uint8_t>& data);

    CanBus* canBus; ///< Pointer to the CanBus instance for communicating with the CAN bus.
    CanRequest* canRequest; ///< Pointer to the CanRequest instance for sending and receiving CAN messages.
};
