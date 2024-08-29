#ifndef CANREQUEST_HPP
#define CANREQUEST_HPP

#include "CanBus.hpp"
#include <functional>

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
     * @brief Sends a CAN message asynchronously over the CAN bus.
     * @param can_id The identifier of the CAN message.
     * @param data The payload data of the CAN message.
     * @param handler A callback to be invoked when the operation completes.
     */
    void sendMessageAsync(uint32_t can_id, const std::vector<uint8_t>& data, std::function<void(bool, const std::vector<uint8_t>&)> handler);

private:
    CanBus& canBus; ///< Reference to the CanBus interface used for communication.
};

#endif // CANREQUEST_HPP
