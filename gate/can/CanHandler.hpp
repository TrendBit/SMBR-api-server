#pragma once
#include <string>
#include <vector>
#include <cstdint>

class CanHandler {
public:
    CanHandler();
    ~CanHandler();

    std::string sendPingRequest(uint32_t can_id, uint8_t seq_number);
    float sendLoadRequest(uint32_t can_id);
    float sendTemperatureRequest(uint32_t can_id);
    bool sendRestartRequest(uint32_t can_id, const std::string& uid);
    bool sendBootloaderRequest(uint32_t can_id, const std::string& uid);
    bool sendHeaterTemperatureRequest(uint32_t can_id, float temperature);
    float sendGetHeaterTemperatureRequest(uint32_t can_id);
    bool sendDisableHeaterRequest(uint32_t can_id);
    float sendTopSensorTemperatureRequest(uint32_t can_id);
    float sendBottomSensorTemperatureRequest(uint32_t can_id);
    bool sendSupplyTypeRequest(uint32_t can_id);
    std::vector<std::string> sendAvailableModulesRequest(uint32_t can_id);
    float sendSystemTemperatureRequest(uint32_t can_id);

private:
    bool sendCanMessage(uint32_t can_id, const uint8_t* data, size_t data_len);
    bool receiveNextCanMessage(std::string &response);
};