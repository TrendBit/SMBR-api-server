#pragma once

#include "base/CommonModule.hpp"
#include <functional>
#include "can/CanRequestManager.hpp"

/**
 * @class CoreModule
 * @brief Singleton class representing the core module in the system.
 */
class CoreModule : public CommonModule {
public:
    static CoreModule& getInstance();

    void getSupplyType(boost::asio::io_context& io_context, std::function<void(bool, bool, bool)> handler);

    ~CoreModule() = default;

private:
    CoreModule();
};
