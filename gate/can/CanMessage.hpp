/**
 * @file CanMessage.hpp
 * @author Vojtěch Mucha
 * @version 0.1
 * @date 23.08.2024
 */

#pragma once

#include <vector>
#include <cstdint>

/**
 * @class CanMessage
 * @brief Represents a CAN message with an identifier and data payload.
 */
class CanMessage {
public:
    /**
     * @brief Constructs a CAN message with the specified identifier and data.
     * @param id The identifier of the CAN message.
     * @param data The payload data of the CAN message.
     */
    CanMessage(uint32_t id, const std::vector<uint8_t>& data);

    /**
     * @brief Gets the identifier of the CAN message.
     * @return The identifier of the CAN message.
     */
    uint32_t getId() const;

    /**
     * @brief Gets the payload data of the CAN message.
     * @return A constant reference to the payload data.
     */
    const std::vector<uint8_t>& getData() const;

    /**
     * @brief Serializes the CAN message into a vector of bytes.
     * @return A vector containing the serialized message.
     */
    std::vector<uint8_t> serialize() const;

    /**
     * @brief Deserializes a raw data vector into a CAN message.
     * @param rawData The raw data vector to deserialize.
     * @return A CanMessage object constructed from the raw data.
     */
    static CanMessage deserialize(const std::vector<uint8_t>& rawData);

private:
    uint32_t id; ///< The identifier of the CAN message.
    std::vector<uint8_t> data; ///< The payload data of the CAN message.
};

