#ifndef CANREQUESTMANAGER_HPP
#define CANREQUESTMANAGER_HPP

#include "CanBus.hpp"
#include "CanRequest.hpp"

class CanRequestManager {
public:
    static CanRequestManager& getInstance();  
    float ping(uint32_t can_id, uint8_t seq_number);

private:
    CanRequestManager(); 
    CanRequestManager(const CanRequestManager&) = delete;
    CanRequestManager& operator=(const CanRequestManager&) = delete;

    CanBus* canBus;
    CanRequest* canRequest;
};

#endif // CANREQUESTMANAGER_HPP
