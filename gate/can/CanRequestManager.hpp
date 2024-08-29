#pragma once

#include "CanRequest.hpp"
#include <boost/asio.hpp>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

/**
 * @class CanRequestManager
 * @brief Singleton class for managing and processing CAN requests.
 *
 * The CanRequestManager class serves as the main interface for sending requests and receiving responses over the CAN bus.
 * It provides fully asynchronous methods for sending messages via CAN and manages a request queue.
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
    static CanRequestManager& getInstance(boost::asio::io_context& io_context);

    /**
     * @brief Asynchronously sends a message over the CAN bus using a request queue.
     * 
     * This method sends a message over the CAN bus asynchronously, managing the requests in a queue.
     * 
     * @param can_id CAN identifier of the message.
     * @param data Data to be sent as the content of the CAN message.
     * @param handler Callback to handle the result of the operation.
     */
    void addRequest(uint32_t can_id, const std::vector<uint8_t>& data, std::function<void(bool, const std::vector<uint8_t>&)> handler);

private:
    /**
     * @brief Private constructor for CanRequestManager.
     * 
     * The constructor is private to prevent the creation of multiple instances of the class. It is used only within `getInstance`.
     */
    CanRequestManager(boost::asio::io_context& io_context);

    /**
     * @brief Processes the request queue.
     * 
     * This method processes requests from the queue in the order they were added and sends them over the CAN bus.
     */
    void processQueue();

    CanRequest* canRequest; ///< Pointer to the CanRequest instance for sending and receiving CAN messages.

    std::queue<std::tuple<uint32_t, std::vector<uint8_t>, std::function<void(bool, const std::vector<uint8_t>&)>>> requestQueue; ///< Queue of CAN requests.
    std::mutex queueMutex; ///< Mutex for synchronizing access to the request queue.
    std::condition_variable queueCondition; ///< Condition variable to signal when there are requests in the queue.
    bool processing; ///< Flag indicating whether the request queue is being processed.
};
