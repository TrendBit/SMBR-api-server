#pragma once

#include <vector>
#include <cstdint>

/**
 * @class CanMessage
 * @brief Class representing a CAN bus message.
 * 
 * CanMessage holds the CAN message ID and the associated data.
 */
class CanMessage {
public:
    /**
     * @brief Constructor for CanMessage.
     * 
     * @param id The CAN message ID.
     * @param data A vector containing the data of the CAN message.
     */
    CanMessage(uint32_t id, const std::vector<uint8_t>& data);

    /**
     * @brief Get the CAN message ID.
     * 
     * @return uint32_t The CAN message ID.
     */
    uint32_t getId() const;

    /**
     * @brief Get the data of the CAN message.
     * 
     * @return const std::vector<uint8_t>& A reference to the data of the CAN message.
     */
    const std::vector<uint8_t>& getData() const;

private:
    uint32_t id;
    std::vector<uint8_t> data; 
};

