#include "CanMessage.hpp"
#include <cstring>

CanMessage::CanMessage(uint32_t id, const std::vector<uint8_t>& data)
    : id(id), data(data) {}

uint32_t CanMessage::getId() const {
    return id;
}

const std::vector<uint8_t>& CanMessage::getData() const {
    return data;
}

std::vector<uint8_t> CanMessage::serialize() const {
    std::vector<uint8_t> result(sizeof(id) + data.size());
    std::memcpy(result.data(), &id, sizeof(id));
    std::memcpy(result.data() + sizeof(id), data.data(), data.size());
    return result;
}

CanMessage CanMessage::deserialize(const std::vector<uint8_t>& rawData) {
    uint32_t id;
    std::memcpy(&id, rawData.data(), sizeof(id));
    std::vector<uint8_t> data(rawData.begin() + sizeof(id), rawData.end());
    return CanMessage(id, data);
}
