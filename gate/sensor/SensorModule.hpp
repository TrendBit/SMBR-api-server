#pragma once

#include "can/CanRequestManager.hpp"
#include <functional>
#include <boost/asio.hpp>
#include "can/CanIdGenerator.hpp"
#include "codes/codes.hpp"
#include <iomanip>

#include "codes/messages/bottle_temperature/temperature_request.hpp"
#include "codes/messages/bottle_temperature/temperature_response.hpp"

/**
 * @class SensorModule
 */
class SensorModule {
public:
    SensorModule(boost::asio::io_context& io_context, CanRequestManager& canRequestManager);

    /**
     * @brief Retrieves the current temperature of the bottle for a specified module.
     * 
     * @param module Target module from which the bottle temperature will be retrieved.
     * @param callback Callback function to handle the retrieved temperature.
     */
    void getBottleTemperature(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback);


protected:
    boost::asio::io_context& m_ioContext; 
    CanRequestManager& m_canRequestManager; 
};