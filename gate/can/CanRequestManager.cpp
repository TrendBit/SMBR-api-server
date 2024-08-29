#include "CanRequestManager.hpp"
#include <thread>
#include <iostream>
#include <iomanip>  

CanRequestManager& CanRequestManager::getInstance(boost::asio::io_context& io_context) {
    static CanRequestManager instance(io_context);
    return instance;
}

CanRequestManager::CanRequestManager(boost::asio::io_context& io_context)
    : canRequest(new CanRequest(*new CanBus(io_context))), processing(false) {
    
    std::cout << "CanRequestManager::CanRequestManager - Before posting to io_context" << std::endl;
    io_context.post([this]() { 
        std::cout << "CanRequestManager::processQueue - This should run in io_context thread" << std::endl;
        processQueue(); 
    });
    std::cout << "CanRequestManager::CanRequestManager - After posting to io_context" << std::endl;
}

void CanRequestManager::addRequest(uint32_t can_id, const std::vector<uint8_t>& data, std::function<void(bool, const std::vector<uint8_t>&)> handler) {
    std::cout << "CanRequestManager::addRequest - CAN ID: 0x" << std::hex << std::setw(8) << std::setfill('0') << can_id 
              << " Data Size: " << std::dec << data.size() << std::endl;

    {
        std::lock_guard<std::mutex> lock(queueMutex);
        requestQueue.emplace(can_id, data, handler);
        std::cout << "CanRequestManager::addRequest - Request queued, queue size: " << requestQueue.size() << std::endl;
    }

    queueCondition.notify_one();
}

void CanRequestManager::processQueue() {
    std::cout << "CanRequestManager::processQueue - Started processing queue" << std::endl;

    while (true) {
        std::tuple<uint32_t, std::vector<uint8_t>, std::function<void(bool, const std::vector<uint8_t>&)>> request;

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            std::cout << "CanRequestManager::processQueue - Waiting for request" << std::endl;

            queueCondition.wait(lock, [this]() { 
                std::cout << "CanRequestManager::processQueue - Checking condition, queue size: " << requestQueue.size() << std::endl;
                return !requestQueue.empty(); 
            });

            std::cout << "CanRequestManager::processQueue - Request found in queue, processing" << std::endl;
            request = std::move(requestQueue.front());
            requestQueue.pop();
        }

        uint32_t can_id = std::get<0>(request);
        std::vector<uint8_t> data = std::get<1>(request);
        auto handler = std::get<2>(request);

        std::cout << "CanRequestManager::processQueue - Sending message, CAN ID: 0x"
                  << std::hex << std::setw(8) << std::setfill('0') << can_id
                  << " Data Size: " << std::dec << data.size() << std::endl;

        canRequest->sendMessageAsync(can_id, data, handler);

        std::cout << "CanRequestManager::processQueue - Message sent, waiting for next request" << std::endl;
    }
}
