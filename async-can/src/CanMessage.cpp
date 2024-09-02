#include "CanMessage.hpp"

CanMessage::CanMessage(uint32_t id, const std::vector<uint8_t>& data)
    : id(id), data(data) {}

uint32_t CanMessage::getId() const {
    return id;
}

const std::vector<uint8_t>& CanMessage::getData() const {
    return data;
}
