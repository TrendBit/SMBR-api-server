#ifndef CANMESSAGE_HPP
#define CANMESSAGE_HPP

#include <vector>
#include <cstdint>

class CanMessage {
public:
    CanMessage(uint32_t id, const std::vector<uint8_t>& data);
    uint32_t getId() const;
    const std::vector<uint8_t>& getData() const;
    std::vector<uint8_t> serialize() const;
    static CanMessage deserialize(const std::vector<uint8_t>& rawData);

private:
    uint32_t id;
    std::vector<uint8_t> data;
};

#endif // CANMESSAGE_HPP
