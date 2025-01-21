#include "ControlModule.hpp"



ControlModule::ControlModule(boost::asio::io_context& io_context, CanRequestManager& canRequestManager)
    : m_ioContext(io_context), m_canRequestManager(canRequestManager) {}


void ControlModule::setIntensity(Codes::Module module, float intensity, int channel, std::function<void(bool)> callback) {

    App_messages::LED_panel::Set_intensity Set_intensity((uint8_t)channel, (float)intensity);

    uint32_t intensity_can_id = createCanId(Set_intensity.Type(), module, Codes::Instance::Exclusive, false);

    m_canRequestManager.sendWithoutResponse(intensity_can_id, Set_intensity.Export_data(), [callback](bool success) {
        callback(success);
    });
}

void ControlModule::getIntensity(CanRequestManager& manager, Codes::Module module, int channel, std::function<void(float)> callback) {
    App_messages::LED_panel::Get_intensity_request getIntensityReq ((uint8_t)channel);

    uint32_t requestCanId = createCanId(getIntensityReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t responseCanId = createCanId(App_messages::LED_panel::Get_intensity_response().Type(), module, Codes::Instance::Exclusive, false);

    double timeoutSeconds = 2;

    manager.addRequest(requestCanId, getIntensityReq.Export_data(), responseCanId, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::LED_panel::Get_intensity_response intensityResponse;
            if (intensityResponse.Interpret_data(dataCopy)) {
                callback(intensityResponse.intensity);  
            } else {
                callback(-1);  
            }
        } else if (status == CanRequestStatus::Timeout) {
            callback(-2); 
        } else {
            callback(-1);  
        }
    }, timeoutSeconds);
}

void ControlModule::getLedTemperature(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback) {
    App_messages::LED_panel::Temperature_request getTemperatureReq;

    uint32_t requestCanId = createCanId(getTemperatureReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t responseCanId = createCanId(App_messages::LED_panel::Temperature_response(0.0f).Type(), module, Codes::Instance::Exclusive, false);

    double timeoutSeconds = 2;

    manager.addRequest(requestCanId, getTemperatureReq.Export_data(), responseCanId, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::LED_panel::Temperature_response temperatureResponse(0.0f);
            if (temperatureResponse.Interpret_data(dataCopy)) {
                float temperature = temperatureResponse.temperature;
                if (temperature < -30) {
                    callback(-100); // Module not available
                } else {
                    callback(temperature); 
                }
            } else {
                callback(-1); // Invalid data
            }
        } else if (status == CanRequestStatus::Timeout) {
            callback(-30); // Timeout 
        } else {
            callback(-1); // General error
        }
    }, timeoutSeconds);
}


void ControlModule::setHeaterIntensity(Codes::Module module, float intensity, std::function<void(bool)> callback) {
    App_messages::Heater::Set_intensity setIntensity(intensity);

    uint32_t intensityCanId = createCanId(setIntensity.Type(), module, Codes::Instance::Exclusive, false);

    m_canRequestManager.sendWithoutResponse(intensityCanId, setIntensity.Export_data(), [callback](bool success) {
        callback(success);
    });
}

void ControlModule::getHeaterIntensity(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback) {
    App_messages::Heater::Get_intensity_request getIntensityReq;

    uint32_t requestCanId = createCanId(getIntensityReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t responseCanId = createCanId(App_messages::Heater::Get_intensity_response(0.0f).Type(), module, Codes::Instance::Exclusive, false);

    double timeoutSeconds = 2;

    manager.addRequest(requestCanId, getIntensityReq.Export_data(), responseCanId, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::Heater::Get_intensity_response intensityResponse(0.0f);
            if (intensityResponse.Interpret_data(dataCopy)) {
                callback(intensityResponse.intensity);  
            } else {
                callback(-1); 
            }
        } else if (status == CanRequestStatus::Timeout) {
            callback(-2);
        } else {
            callback(-1); 
        }
    }, timeoutSeconds);
}

void ControlModule::setHeaterTargetTemperature(Codes::Module module, float targetTemperature, std::function<void(bool)> callback) {
    App_messages::Heater::Set_target_temperature setTargetTemperature(targetTemperature);

    uint32_t targetTempCanId = createCanId(setTargetTemperature.Type(), module, Codes::Instance::Exclusive, false);

    m_canRequestManager.sendWithoutResponse(targetTempCanId, setTargetTemperature.Export_data(), [callback](bool success) {
        callback(success);
    });
}

void ControlModule::getHeaterTargetTemperature(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback) {
    App_messages::Heater::Get_target_temperature_request getTargetTempReq;

    uint32_t requestCanId = createCanId(getTargetTempReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t responseCanId = createCanId(App_messages::Heater::Get_target_temperature_response(0.0f).Type(), module, Codes::Instance::Exclusive, false);

    double timeoutSeconds = 2;

    manager.addRequest(requestCanId, getTargetTempReq.Export_data(), responseCanId, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::Heater::Get_target_temperature_response targetTempResponse(0.0f);
            if (targetTempResponse.Interpret_data(dataCopy)) {
                float temperature = targetTempResponse.temperature;
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

void ControlModule::getHeaterPlateTemperature(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback) {
    App_messages::Heater::Get_plate_temperature_request getPlateTempReq;

    uint32_t requestCanId = createCanId(getPlateTempReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t responseCanId = createCanId(App_messages::Heater::Get_plate_temperature_response(0.0f).Type(), module, Codes::Instance::Exclusive, false);

    double timeoutSeconds = 2;

    manager.addRequest(requestCanId, getPlateTempReq.Export_data(), responseCanId, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::Heater::Get_plate_temperature_response plateTempResponse(0.0f);
            if (plateTempResponse.Interpret_data(dataCopy)) {
                float temperature = plateTempResponse.temperature;
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


void ControlModule::turnOffHeater(Codes::Module module, std::function<void(bool)> callback) {
    App_messages::Heater::Heater_turn_off turnOff;

    uint32_t turnOffCanId = createCanId(turnOff.Type(), module, Codes::Instance::Exclusive, false);

    m_canRequestManager.sendWithoutResponse(turnOffCanId, turnOff.Export_data(), [callback](bool success) {
        callback(success);
    });
}

void ControlModule::setCuvettePumpSpeed(Codes::Module module, float speed, std::function<void(bool)> callback) {
    App_messages::Cuvette_pump::Set_speed setSpeed(speed);

    uint32_t speedCanId = createCanId(setSpeed.Type(), module, Codes::Instance::Exclusive, false);

    m_canRequestManager.sendWithoutResponse(speedCanId, setSpeed.Export_data(), [callback](bool success) {
        callback(success);
    });
}

void ControlModule::getCuvettePumpSpeed(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback) {
    App_messages::Cuvette_pump::Get_speed_request getSpeedReq;

    uint32_t requestCanId = createCanId(getSpeedReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t responseCanId = createCanId(App_messages::Cuvette_pump::Get_speed_response(0.0f).Type(), module, Codes::Instance::Exclusive, false);

    double timeoutSeconds = 2;

    manager.addRequest(requestCanId, getSpeedReq.Export_data(), responseCanId, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::Cuvette_pump::Get_speed_response speedResponse(0.0f);
            if (speedResponse.Interpret_data(dataCopy)) {
                callback(speedResponse.speed);  
            } else {
                callback(-1); 
            }
        } else if (status == CanRequestStatus::Timeout) {
            callback(-2);
        } else {
            callback(-1); 
        }
    }, timeoutSeconds);
}

void ControlModule::setCuvettePumpFlowrate(Codes::Module module, float flowrate, std::function<void(bool)> callback) {
    App_messages::Cuvette_pump::Set_flowrate setFlowrate(flowrate);

    uint32_t flowrateCanId = createCanId(setFlowrate.Type(), module, Codes::Instance::Exclusive, false);

    m_canRequestManager.sendWithoutResponse(flowrateCanId, setFlowrate.Export_data(), [callback](bool success) {
        callback(success);
    });
}

void ControlModule::getCuvettePumpFlowrate(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback) {
    App_messages::Cuvette_pump::Get_flowrate_request getFlowrateReq;

    uint32_t requestCanId = createCanId(getFlowrateReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t responseCanId = createCanId(App_messages::Cuvette_pump::Get_flowrate_response(0.0f).Type(), module, Codes::Instance::Exclusive, false);

    double timeoutSeconds = 2;

    manager.addRequest(requestCanId, getFlowrateReq.Export_data(), responseCanId, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::Cuvette_pump::Get_flowrate_response flowrateResponse(0.0f);
            if (flowrateResponse.Interpret_data(dataCopy)) {
                callback(flowrateResponse.flowrate);  
            } else {
                callback(-1); 
            }
        } else if (status == CanRequestStatus::Timeout) {
            callback(-2000); 
        } else {
            callback(-1); 
        }
    }, timeoutSeconds);
}

void ControlModule::moveCuvettePump(Codes::Module module, float volume, float flowrate, std::function<void(bool)> callback) {
    App_messages::Cuvette_pump::Move moveReq(volume, flowrate);

    uint32_t moveCanId = createCanId(moveReq.Type(), module, Codes::Instance::Exclusive, false);

    m_canRequestManager.sendWithoutResponse(moveCanId, moveReq.Export_data(), [callback](bool success) {
        callback(success);
    });
}

void ControlModule::primeCuvettePump(Codes::Module module, std::function<void(bool)> callback) {
    App_messages::Cuvette_pump::Prime primeMessage;

    uint32_t primeCanId = createCanId(primeMessage.Type(), module, Codes::Instance::Exclusive, false);

    m_canRequestManager.sendWithoutResponse(primeCanId, primeMessage.Export_data(), [callback](bool success) {
        callback(success);
    });
}

void ControlModule::purgeCuvettePump(Codes::Module module, std::function<void(bool)> callback) {
    App_messages::Cuvette_pump::Purge purgeMessage;

    uint32_t purgeCanId = createCanId(purgeMessage.Type(), module, Codes::Instance::Exclusive, false);

    m_canRequestManager.sendWithoutResponse(purgeCanId, purgeMessage.Export_data(), [callback](bool success) {
        callback(success);
    });
}

void ControlModule::stopCuvettePump(Codes::Module module, std::function<void(bool)> callback) {
    App_messages::Cuvette_pump::Stop stopMessage;

    uint32_t stopCanId = createCanId(stopMessage.Type(), module, Codes::Instance::Exclusive, false);

    m_canRequestManager.sendWithoutResponse(stopCanId, stopMessage.Export_data(), [callback](bool success) {
        callback(success);
    });
}

void ControlModule::setAeratorSpeed(Codes::Module module, float speed, std::function<void(bool)> callback) {
    App_messages::Aerator::Set_speed setSpeed(speed);

    uint32_t speedCanId = createCanId(setSpeed.Type(), module, Codes::Instance::Exclusive, false);

    m_canRequestManager.sendWithoutResponse(speedCanId, setSpeed.Export_data(), [callback](bool success) {
        callback(success);
    });
}

void ControlModule::getAeratorSpeed(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback) {
    App_messages::Aerator::Get_speed_request getSpeedReq;

    uint32_t requestCanId = createCanId(getSpeedReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t responseCanId = createCanId(App_messages::Aerator::Get_speed_response(0.0f).Type(), module, Codes::Instance::Exclusive, false);

    double timeoutSeconds = 2;

    manager.addRequest(requestCanId, getSpeedReq.Export_data(), responseCanId, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::Aerator::Get_speed_response speedResponse(0.0f);
            if (speedResponse.Interpret_data(dataCopy)) {
                callback(speedResponse.speed);  
            } else {
                callback(-1); 
            }
        } else if (status == CanRequestStatus::Timeout) {
            callback(-2);
        } else {
            callback(-1); 
        }
    }, timeoutSeconds);
}

void ControlModule::setAeratorFlowrate(Codes::Module module, float flowrate, std::function<void(bool)> callback) {
    App_messages::Aerator::Set_flowrate setFlowrate(flowrate);

    uint32_t flowrateCanId = createCanId(setFlowrate.Type(), module, Codes::Instance::Exclusive, false);

    m_canRequestManager.sendWithoutResponse(flowrateCanId, setFlowrate.Export_data(), [callback](bool success) {
        callback(success);
    });
}

void ControlModule::getAeratorFlowrate(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback) {
    App_messages::Aerator::Get_flowrate_request getFlowrateReq;

    uint32_t requestCanId = createCanId(getFlowrateReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t responseCanId = createCanId(App_messages::Aerator::Get_flowrate_response(0.0f).Type(), module, Codes::Instance::Exclusive, false);

    double timeoutSeconds = 2;

    manager.addRequest(requestCanId, getFlowrateReq.Export_data(), responseCanId, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::Aerator::Get_flowrate_response flowrateResponse(0.0f);
            if (flowrateResponse.Interpret_data(dataCopy)) {
                callback(flowrateResponse.flowrate);  
            } else {
                callback(-1); 
            }
        } else if (status == CanRequestStatus::Timeout) {
            callback(-2);
        } else {
            callback(-1); 
        }
    }, timeoutSeconds);
}

void ControlModule::moveAerator(Codes::Module module, float volume, float flowrate, std::function<void(bool)> callback) {
    App_messages::Aerator::Move moveReq(volume, flowrate);

    uint32_t moveCanId = createCanId(moveReq.Type(), module, Codes::Instance::Exclusive, false);

    m_canRequestManager.sendWithoutResponse(moveCanId, moveReq.Export_data(), [callback](bool success) {
        callback(success);
    });
}

void ControlModule::stopAerator(Codes::Module module, std::function<void(bool)> callback) {
    App_messages::Aerator::Stop stopMessage;

    uint32_t stopCanId = createCanId(stopMessage.Type(), module, Codes::Instance::Exclusive, false);

    m_canRequestManager.sendWithoutResponse(stopCanId, stopMessage.Export_data(), [callback](bool success) {
        callback(success);
    });
}

void ControlModule::setMixerSpeed(Codes::Module module, float speed, std::function<void(bool)> callback) {
    App_messages::Mixer::Set_speed setSpeed(speed);

    uint32_t speedCanId = createCanId(setSpeed.Type(), module, Codes::Instance::Exclusive, false);

    m_canRequestManager.sendWithoutResponse(speedCanId, setSpeed.Export_data(), [callback](bool success) {
        callback(success);
    });
}

void ControlModule::getMixerSpeed(CanRequestManager& manager, Codes::Module module, std::function<void(float)> callback) {
    App_messages::Mixer::Get_speed_request getSpeedReq;

    uint32_t requestCanId = createCanId(getSpeedReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t responseCanId = createCanId(App_messages::Mixer::Get_speed_response(0.0f).Type(), module, Codes::Instance::Exclusive, false);

    double timeoutSeconds = 2;

    manager.addRequest(requestCanId, getSpeedReq.Export_data(), responseCanId, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::Mixer::Get_speed_response speedResponse(0.0f);
            if (speedResponse.Interpret_data(dataCopy)) {
                callback(speedResponse.speed);
            } else {
                callback(-1);
            }
        } else if (status == CanRequestStatus::Timeout) {
            callback(-2);
        } else {
            callback(-1); 
        }
    }, timeoutSeconds);
}

void ControlModule::setMixerRpm(Codes::Module module, float rpm, std::function<void(bool)> callback) {
    App_messages::Mixer::Set_rpm setRpm(rpm);

    uint32_t rpmCanId = createCanId(setRpm.Type(), module, Codes::Instance::Exclusive, false);

    m_canRequestManager.sendWithoutResponse(rpmCanId, setRpm.Export_data(), [callback](bool success) {
        callback(success);
    });
}

void ControlModule::getMixerRpm(CanRequestManager& manager, Codes::Module module, std::function<void(int)> callback) {
    App_messages::Mixer::Get_rpm_request getRpmReq;

    uint32_t requestCanId = createCanId(getRpmReq.Type(), module, Codes::Instance::Exclusive, false);
    uint32_t responseCanId = createCanId(App_messages::Mixer::Get_rpm_response(0).Type(), module, Codes::Instance::Exclusive, false);

    double timeoutSeconds = 2;

    manager.addRequest(requestCanId, getRpmReq.Export_data(), responseCanId, [callback](CanRequestStatus status, const CanMessage& response) {
        if (status == CanRequestStatus::Success) {
            can_data_vector_t dataCopy = response.getData();

            App_messages::Mixer::Get_rpm_response rpmResponse(0);
            if (rpmResponse.Interpret_data(dataCopy)) {
                callback(rpmResponse.rpm);  
            } else {
                callback(-1); 
            }
        } else if (status == CanRequestStatus::Timeout) {
            callback(-2);
        } else {
            callback(-1); 
        }
    }, timeoutSeconds);
}
