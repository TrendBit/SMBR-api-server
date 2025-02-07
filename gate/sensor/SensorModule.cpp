#include "SensorModule.hpp"

extern backward::SignalHandling sh;

SensorModule::SensorModule(boost::asio::io_context& io_context, CanRequestManager& canRequestManager)
    : m_ioContext(io_context), m_canRequestManager(canRequestManager) {}


void SensorModule::getBottleTemperature(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback) {
    App_messages::Bottle_temperature::Temperature_request getTemperatureReq;

    uint32_t requestCanId = createCanId(getTemperatureReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t responseCanId = createCanId(App_messages::Bottle_temperature::Temperature_response().Type(), module, Codes::Instance::Exclusive, false);

    double timeoutSeconds = 2;

    manager.addRequest(requestCanId, getTemperatureReq.Export_data(), responseCanId, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::Bottle_temperature::Temperature_response temperatureResponse(0);
            if (temperatureResponse.Interpret_data(dataCopy)) {
                float temperature = temperatureResponse.temperature;
                if (temperature < -30) {
                    callback(-100); 
                } else {
                    callback(temperature); 
                }
            } else {
                callback(-1); 
            }
        } else if (status == CanRequestStatus::Timeout) {
            callback(-30); 
        } else {
            callback(-1);
        }
    }, timeoutSeconds);
}

void SensorModule::getTopMeasuredTemperature(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback) {
    App_messages::Bottle_temperature::Top_measured_temperature_request getTemperatureReq;

    uint32_t requestCanId = createCanId(getTemperatureReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t responseCanId = createCanId(App_messages::Bottle_temperature::Top_measured_temperature_response().Type(), module, Codes::Instance::Exclusive, false);

    double timeoutSeconds = 2;

    manager.addRequest(requestCanId, getTemperatureReq.Export_data(), responseCanId, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::Bottle_temperature::Top_measured_temperature_response temperatureResponse(0);
            if (temperatureResponse.Interpret_data(dataCopy)) {
                float temperature = temperatureResponse.temperature;
                if (temperature < -30) {
                    callback(-100); 
                } else {
                    callback(temperature); 
                }
            } else {
                callback(-1); 
            }
        } else if (status == CanRequestStatus::Timeout) {
            callback(-30); 
        } else {
            callback(-1); 
        }
    }, timeoutSeconds);
}

void SensorModule::getBottomMeasuredTemperature(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback) {
    App_messages::Bottle_temperature::Bottom_measured_temperature_request getTemperatureReq;

    uint32_t requestCanId = createCanId(getTemperatureReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t responseCanId = createCanId(App_messages::Bottle_temperature::Bottom_measured_temperature_response().Type(), module, Codes::Instance::Exclusive, false);

    double timeoutSeconds = 2;

    manager.addRequest(requestCanId, getTemperatureReq.Export_data(), responseCanId, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::Bottle_temperature::Bottom_measured_temperature_response temperatureResponse(0);
            if (temperatureResponse.Interpret_data(dataCopy)) {
                float temperature = temperatureResponse.temperature;
                if (temperature < -30) {
                    callback(-100); 
                } else {
                    callback(temperature); 
                }
            } else {
                callback(-1); 
            }
        } else if (status == CanRequestStatus::Timeout) {
            callback(-30); 
        } else {
            callback(-1); 
        }
    }, timeoutSeconds);
}

void SensorModule::getTopSensorTemperature(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback) {
    App_messages::Bottle_temperature::Top_sensor_temperature_request getTemperatureReq;

    uint32_t requestCanId = createCanId(getTemperatureReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t responseCanId = createCanId(App_messages::Bottle_temperature::Top_sensor_temperature_response().Type(), module, Codes::Instance::Exclusive, false);

    double timeoutSeconds = 2;

    manager.addRequest(requestCanId, getTemperatureReq.Export_data(), responseCanId, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::Bottle_temperature::Top_sensor_temperature_response temperatureResponse(0);
            if (temperatureResponse.Interpret_data(dataCopy)) {
                float temperature = temperatureResponse.temperature;
                if (temperature < -30) {
                    callback(-100);
                } else {
                    callback(temperature); 
                }
            } else {
                callback(-1); 
            }
        } else if (status == CanRequestStatus::Timeout) {
            callback(-30); 
        } else {
            callback(-1); 
        }
    }, timeoutSeconds);
}

void SensorModule::getBottomSensorTemperature(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback) {
    App_messages::Bottle_temperature::Bottom_sensor_temperature_request getTemperatureReq;

    uint32_t requestCanId = createCanId(getTemperatureReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t responseCanId = createCanId(App_messages::Bottle_temperature::Bottom_sensor_temperature_response().Type(), module, Codes::Instance::Exclusive, false);

    double timeoutSeconds = 2;

    manager.addRequest(requestCanId, getTemperatureReq.Export_data(), responseCanId, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::Bottle_temperature::Bottom_sensor_temperature_response temperatureResponse(0);
            if (temperatureResponse.Interpret_data(dataCopy)) {
                float temperature = temperatureResponse.temperature;
                if (temperature < -30) {
                    callback(-100); 
                } else {
                    callback(temperature); 
                }
            } else {
                callback(-1); 
            }
        } else if (status == CanRequestStatus::Timeout) {
            callback(-30);
        } else {
            callback(-1); 
        }
    }, timeoutSeconds);
}

