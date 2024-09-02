#include "CanBus.hpp"
#include "codes.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <functional>

uint32_t createCanId(Codes::Message_type messageType, Codes::Module module, Codes::Instance instance, bool emergencyFlag) {
    uint32_t id = 0;
    id |= (emergencyFlag ? 1 : 0) << 28;
    id |= static_cast<uint32_t>(messageType) << 16;
    id |= static_cast<uint32_t>(module) << 4;
    id |= static_cast<uint32_t>(instance);
    return id;
}

void sendPingsDuringWait(CanBus& canBus, std::function<void(bool, const CanMessage&)> receiveHandler) {
    uint32_t ping_can_id = createCanId(Codes::Message_type::Ping_request, 
                                       Codes::Module::Control_board, 
                                       Codes::Instance::Exclusive, 
                                       false);

    canBus.asyncReceive(receiveHandler);

    for (int i = 1; i <= 5; ++i) {
        std::vector<uint8_t> ping_data = {static_cast<uint8_t>(i)};
        CanMessage pingMessage(ping_can_id, ping_data);

        canBus.asyncSend(pingMessage, [](bool success) {
            if (success) {
               // std::cout << "Ping message sent successfully" << std::endl;
            } else {
                std::cerr << "Failed to send Ping message" << std::endl;
            }
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(900));
    }
}

int main() {
    try {
        boost::asio::io_context io_context;

        CanBus canBus(io_context);

        uint32_t probe_can_id = createCanId(Codes::Message_type::Probe_modules_request, 
                                            Codes::Module::All, 
                                            Codes::Instance::Exclusive, 
                                            false);
        std::vector<uint8_t> probe_data = {};
        CanMessage probeMessage(probe_can_id, probe_data);

        std::function<void(bool, const CanMessage&)> receiveHandler;
        receiveHandler = [&](bool success, const CanMessage& message) {
            if (success) {
                //std::cout << "Received CAN message with ID: 0x" << std::hex << message.getId() << std::endl;
            } else {
                std::cerr << "Failed to receive CAN message." << std::endl;
            }
            canBus.asyncReceive(receiveHandler);
        };

        canBus.asyncReceive(receiveHandler);

        std::thread receiveThread([&io_context]() {
            io_context.run();
        });

        canBus.asyncSend(probeMessage, [](bool success) {
            if (success) {
               // std::cout << "Probe_modules_request message sent successfully" << std::endl;
            } else {
                std::cerr << "Failed to send Probe_modules_request message" << std::endl;
            }
        });

        sendPingsDuringWait(canBus, receiveHandler);

        io_context.run();

        receiveThread.join();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}