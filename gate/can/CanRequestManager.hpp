#ifndef CANREQUESTMANAGER_HPP
#define CANREQUESTMANAGER_HPP

#include "CanBus.hpp"
#include "CanRequest.hpp"
#include <utility>
#include <vector>

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
     * @brief Sends a CAN message with the given ID and data.
     * @param can_id The identifier of the CAN message.
     * @param data The payload data of the CAN message.
     * @return A pair containing a success flag and the received data as a vector of bytes.
     */
    std::pair<bool, std::vector<uint8_t>> sendMessage(uint32_t can_id, const std::vector<uint8_t>& data);

private:
    /**
     * @brief Private constructor to prevent direct instantiation.
     */
    CanRequestManager();

    CanBus* canBus; ///< Pointer to the CanBus interface used for communication.
    CanRequest* canRequest; ///< Pointer to the CanRequest object used for creating and sending requests.
};

#endif // CANREQUESTMANAGER_HPP
