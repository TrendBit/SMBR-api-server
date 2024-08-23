#ifndef CANSERVICE_HPP
#define CANSERVICE_HPP

#include "CanBus.hpp"
#include "CanRequest.hpp"

class CanService {
public:
    CanService();
    float ping(uint32_t can_id, uint8_t seq_number);

private:
    CanBus canBus;
    CanRequest canRequest;
};

#endif // CANSERVICE_HPP
