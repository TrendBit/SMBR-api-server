#ifndef CANMESSAGE_HPP
#define CANMESSAGE_HPP

#include <vector>
#include <cstdint>

class CanMessage {
public:
    CanMessage(uint32_t id, const std::vector<uint8_t>& data);

    uint32_t getId() const;
    const std::vector<uint8_t>& getData() const;

private:
    uint32_t id;
    std::vector<uint8_t> data;
};

#endif 
