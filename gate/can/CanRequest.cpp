#include "CanRequest.hpp"
#include <iostream>
#include <iomanip>  

CanRequest::CanRequest(CanBus& canBus) : canBus(canBus) {
    std::cout << "CanRequest::CanRequest - Initialized" << std::endl;
}

void CanRequest::sendMessageAsync(uint32_t can_id, const std::vector<uint8_t>& data, std::function<void(bool, const std::vector<uint8_t>&)> handler) {
    std::cout << "CanRequest::sendMessageAsync - Sending CAN message, CAN ID: 0x" 
              << std::hex << std::setw(8) << std::setfill('0') << can_id 
              << " Data Size: " << std::dec << data.size() << std::endl;

    CanMessage request(can_id, data);

    canBus.asyncSend(request, [this, handler](bool success) {
        std::cout << "CanRequest::sendMessageAsync - asyncSend handler called, success: " << success << std::endl;

        if (!success) {
            std::cout << "CanRequest::sendMessageAsync - asyncSend failed" << std::endl;
            handler(false, {});
            return;
        }

        std::cout << "CanRequest::sendMessageAsync - asyncSend succeeded, about to call asyncReceive" << std::endl;

        canBus.asyncReceive([handler](bool success, const CanMessage& response) {
            std::cout << "CanRequest::sendMessageAsync - asyncReceive handler called, success: " << success << std::endl;

            if (success) {
                std::cout << "CanRequest::sendMessageAsync - Response received, Data Size: " 
                          << response.getData().size() << std::endl;
                handler(true, response.getData());
            } else {
                std::cout << "CanRequest::sendMessageAsync - asyncReceive failed" << std::endl;
                handler(false, {});
            }
        });

        std::cout << "CanRequest::sendMessageAsync - asyncReceive called" << std::endl;
    });

    std::cout << "CanRequest::sendMessageAsync - asyncSend called" << std::endl;
}
