#pragma once

#include <cstdint>
#include <string>
#include <functional>
#include <boost/asio.hpp>
#include "codes/codes.hpp"
#include "can/CanIdGenerator.hpp"

/**
 * @class CommonModule
 * @brief Base class for modules that interact with the CAN bus.
 */
class CommonModule {
public:
    CommonModule(Codes::Module mod, Codes::Instance inst);
    virtual ~CommonModule();

    /**
     * @brief Sends a ping request to the associated module over the CAN bus.
     * @param handler Callback function to handle the response time.
     */
    void ping(boost::asio::io_context& io_context, std::function<void(float)> handler);

    /**
     * @brief Sends a load request to the associated module over the CAN bus and processes the response data.
     * @param handler Callback function to handle the load and cores.
     */
    void getLoad(boost::asio::io_context& io_context,std::function<void(bool, float, int)> handler);

    /**
     * @brief Sends a core temperature request to the associated module over the CAN bus and processes the response data.
     * @param handler Callback function to handle the core temperature.
     */
    void getCoreTemp(boost::asio::io_context& io_context, std::function<void(bool, float)> handler);

    void restart(boost::asio::io_context& io_context, const std::string& uid, std::function<void(bool)> handler);
    void bootloader(boost::asio::io_context& io_context, const std::string& uid, std::function<void(bool)> handler);

protected:
    Codes::Module module;
    Codes::Instance instance;
};
