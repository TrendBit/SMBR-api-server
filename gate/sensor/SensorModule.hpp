#pragma once

#include "base/CommonModule.hpp"
#include <functional>
#include "can/CanRequestManager.hpp"

/**
 * @class SensorModule
 * @brief Singleton class representing the sensor module in the system.
 */
class SensorModule : public CommonModule {
public:
    static SensorModule& getInstance();

    void getTopTemperature(boost::asio::io_context& io_context, std::function<void(float)> handler);
    void getBottomTemperature(boost::asio::io_context& io_context, std::function<void(float)> handler);

    ~SensorModule() = default;

private:
    SensorModule();
};
