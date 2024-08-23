#include "CanIdGenerator.hpp"

uint32_t createCanId(Codes::Message_type messageType, Codes::Module module, Codes::Instance instance, bool emergencyFlag) {
    uint32_t id = 0;
    id |= (emergencyFlag ? 1 : 0) << 28;
    id |= static_cast<uint32_t>(messageType) << 16;
    id |= static_cast<uint32_t>(module) << 4;
    id |= static_cast<uint32_t>(instance);
    return id;
}
