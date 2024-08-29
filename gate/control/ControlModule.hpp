#pragma once

#include "base/CommonModule.hpp"
#include <functional>
#include <boost/asio.hpp>
#include "can/CanRequestManager.hpp"

/**
 * @class ControlModule
 * @brief Singleton class representing the control module in the system.
 */
class ControlModule : public CommonModule {
public:
    /**
     * @brief Retrieves the singleton instance of ControlModule.
     * 
     * @return Reference to the singleton instance of ControlModule.
     */
    static ControlModule& getInstance();

    void setHeaterTemperature(boost::asio::io_context& io_context, float temperature, std::function<void(bool)> handler);
    void getHeaterTemperature(boost::asio::io_context& io_context, std::function<void(float)> handler);
    void disableHeater(boost::asio::io_context& io_context, std::function<void(bool)> handler);

    ~ControlModule() = default;

private:
    ControlModule();
};
