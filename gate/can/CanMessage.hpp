#ifndef CANMESSAGE_HPP
#define CANMESSAGE_HPP

#include <vector>
#include <cstdint>

/**
 * @class CanMessage
 * @brief Represents a CAN bus message.
 */
class CanMessage {
public:
    /**
     * @brief Constructs a CanMessage object.
     * @param id The CAN message identifier.
     * @param data The payload data of the CAN message.
     */
    CanMessage(uint32_t id, const std::vector<uint8_t>& data);

    /**
     * @brief Gets the CAN message identifier.
     * @return The CAN message identifier.
     */
    uint32_t getId() const;

    /**
     * @brief Gets the payload data of the CAN message.
     * @return The payload data as a vector of bytes.
     */
    const std::vector<uint8_t>& getData() const;

    /**
     * @brief Serializes the CAN message to a byte array.
     * @return The serialized CAN message as a vector of bytes.
     */
    std::vector<uint8_t> serialize() const;

    /**
     * @brief Deserializes a byte array into a CAN message.
     * @param rawData The serialized CAN message as a vector of bytes.
     * @return The deserialized CAN message.
     */
    static CanMessage deserialize(const std::vector<uint8_t>& rawData);

private:
    uint32_t id; ///< The CAN message identifier.
    std::vector<uint8_t> data; ///< The payload data of the CAN message.
};

#endif // CANMESSAGE_HPP
