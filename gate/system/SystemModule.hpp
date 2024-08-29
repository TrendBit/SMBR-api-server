#pragma once

#include <vector>
#include <string>
#include <optional>
#include <functional>
#include "can/CanRequestManager.hpp"
#include "can/CanIdGenerator.hpp"

/**
 * @class SystemModule
 * @brief Singleton class representing the system module, providing information about available modules and system temperature.
 */
class SystemModule {
public:
    static SystemModule& getInstance();

    void getAvailableModules(boost::asio::io_context& io_context, std::function<void(std::optional<std::vector<uint8_t>>)> handler);
    void getSystemTemperature(boost::asio::io_context& io_context, std::function<void(float)> handler);

    ~SystemModule() = default;

private:
    SystemModule() = default;
};
