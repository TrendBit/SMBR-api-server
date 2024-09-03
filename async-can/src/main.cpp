#include "CanBus.hpp"
#include "CanRequestManager.hpp"
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
void sendPingControlRequest(CanRequestManager& manager) {
    uint32_t ping_can_id = createCanId(Codes::Message_type::Ping_request, 
                                       Codes::Module::Control_board, 
                                       Codes::Instance::Exclusive, 
                                       false);
    uint32_t ping_response_id = createCanId(Codes::Message_type::Ping_response, 
                                            Codes::Module::Control_board, 
                                            Codes::Instance::Exclusive, 
                                            false);
    std::vector<uint8_t> ping_data = {0x01};  

    manager.addRequest(ping_can_id, ping_data, ping_response_id, [](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            std::cout << "Ping Control - Received response with ID: 0x" << std::hex << response.getId() << " and data: ";
            for (const auto& byte : response.getData()) {
                std::cout << std::hex << static_cast<int>(byte) << " ";
            }
            std::cout << std::endl;
        } else if (status == CanRequestStatus::Timeout) {
            std::cerr << "Ping Control - Request timed out." << std::endl;
        } else {
            std::cerr << "Ping Control - Failed to receive response." << std::endl;
        }
    });
}

void sendPingControl2Request(CanRequestManager& manager) {
    uint32_t ping_can_id = createCanId(Codes::Message_type::Ping_request, 
                                       Codes::Module::Control_board, 
                                       Codes::Instance::Exclusive, 
                                       false);
    uint32_t ping_response_id = createCanId(Codes::Message_type::Ping_response, 
                                            Codes::Module::Control_board, 
                                            Codes::Instance::Exclusive, 
                                            false);
    std::vector<uint8_t> ping_data = {0x02}; 

    manager.addRequest(ping_can_id, ping_data, ping_response_id, [](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            std::cout << "Ping Control2 - Received response with ID: 0x" << std::hex << response.getId() << " and data: ";
            for (const auto& byte : response.getData()) {
                std::cout << std::hex << static_cast<int>(byte) << " ";
            }
            std::cout << std::endl;
        } else if (status == CanRequestStatus::Timeout) {
            std::cerr << "Ping Control2 - Request timed out." << std::endl;
        } else {
            std::cerr << "Ping Control2 - Failed to receive response." << std::endl;
        }
    });
}

void sendPingControl3Request(CanRequestManager& manager) {
    uint32_t ping_can_id = createCanId(Codes::Message_type::Ping_request, 
                                       Codes::Module::Control_board, 
                                       Codes::Instance::Exclusive, 
                                       false);
    uint32_t ping_response_id = createCanId(Codes::Message_type::Ping_response, 
                                            Codes::Module::Control_board, 
                                            Codes::Instance::Exclusive, 
                                            false);
    std::vector<uint8_t> ping_data = {0x03};  

    manager.addRequest(ping_can_id, ping_data, ping_response_id, [](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            std::cout << "Ping Control3 - Received response with ID: 0x" << std::hex << response.getId() << " and data: ";
            for (const auto& byte : response.getData()) {
                std::cout << std::hex << static_cast<int>(byte) << " ";
            }
            std::cout << std::endl;
        } else if (status == CanRequestStatus::Timeout) {
            std::cerr << "Ping Control3 - Request timed out." << std::endl;
        } else {
            std::cerr << "Ping Control3 - Failed to receive response." << std::endl;
        }
    });
}

void sendPingSenzorRequest(CanRequestManager& manager) {
    uint32_t ping_can_id = createCanId(Codes::Message_type::Ping_request, 
                                       Codes::Module::Sensor_board, 
                                       Codes::Instance::Exclusive, 
                                       false);
    uint32_t ping_response_id = createCanId(Codes::Message_type::Ping_response, 
                                            Codes::Module::Sensor_board, 
                                            Codes::Instance::Exclusive, 
                                            false);
    std::vector<uint8_t> ping_data = {0x05}; 

    manager.addRequest(ping_can_id, ping_data, ping_response_id, [](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            std::cout << "Ping Senzor - Received response with ID: 0x" << std::hex << response.getId() << " and data: ";
            for (const auto& byte : response.getData()) {
                std::cout << std::hex << static_cast<int>(byte) << " ";
            }
            std::cout << std::endl;
        } else if (status == CanRequestStatus::Timeout) {
            std::cerr << "Ping Senzor - Request timed out." << std::endl;
        } else {
            std::cerr << "Ping Senzor - Failed to receive response." << std::endl;
        }
    });
}

void sendTempRequest(CanRequestManager& manager) {
    uint32_t temp_can_id = createCanId(Codes::Message_type::Core_temperature_request, 
                                       Codes::Module::Control_board, 
                                       Codes::Instance::Exclusive, 
                                       false);
    uint32_t temp_response_id = createCanId(Codes::Message_type::Core_temperature_response, 
                                            Codes::Module::Control_board, 
                                            Codes::Instance::Exclusive, 
                                            false);
    std::vector<uint8_t> temp_data = {};  

    manager.addRequest(temp_can_id, temp_data, 0x203, [](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            std::cout << "Temp - Received response with ID: 0x" << std::hex << response.getId() << " and data: ";
            for (const auto& byte : response.getData()) {
                std::cout << std::hex << static_cast<int>(byte) << " ";
            }
            std::cout << std::endl;
        } else if (status == CanRequestStatus::Timeout) {
            std::cerr << "Temp - Request timed out." << std::endl;
        } else {
            std::cerr << "Temp - Failed to receive response." << std::endl;
        }
    });
}

int main() {
    try {
        boost::asio::io_context io_context;

        CanBus canBus(io_context);
        CanRequestManager manager(io_context, canBus);

        std::thread ioThread([&io_context]() {
            std::cout << "Starting io_context.run()" << std::endl;
            io_context.run();
            std::cout << "io_context.run() finished" << std::endl;
        });

        sendPingSenzorRequest(manager);
        std::this_thread::sleep_for(std::chrono::milliseconds(300)); 
        sendPingControlRequest(manager);
        std::this_thread::sleep_for(std::chrono::milliseconds(300)); 
        sendPingControl2Request(manager);
        std::this_thread::sleep_for(std::chrono::milliseconds(300)); 
        sendPingSenzorRequest(manager);
        std::this_thread::sleep_for(std::chrono::milliseconds(300)); 
        sendPingControl3Request(manager);
        std::this_thread::sleep_for(std::chrono::milliseconds(300)); 
        sendPingSenzorRequest(manager);
        std::this_thread::sleep_for(std::chrono::milliseconds(300)); 
        sendTempRequest(manager);

        ioThread.join();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
