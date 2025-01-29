#include "DummyCanBus.hpp"
#include <iostream>

DummyCanBus::DummyCanBus(boost::asio::io_context& io_context) :
    ioContext(io_context) {

    std::cout << "DummyCanBus created" << std::endl;
}

DummyCanBus::~DummyCanBus() {
    std::cout << "DummyCanBus removed" << std::endl;
}

void DummyCanBus::asyncSend(const CanMessage& message, std::function<void(bool)> handler) {
    ioContext.post([handler]() {
        std::cout << "Dummy asyncSend handler called" << std::endl;
        handler(false);
    });
    
}

void DummyCanBus::asyncReceive(std::function<void(bool, const CanMessage&)> handler) {
    ioContext.post([handler]() {
        //std::cout << "Failed to receive dummy can message" << std::endl;
        handler(true, CanMessage(0, {}));
    });
}
