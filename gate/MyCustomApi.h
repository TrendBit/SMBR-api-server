#ifndef MY_CUSTOM_API_H
#define MY_CUSTOM_API_H

#include "OAIDefaultApi.h"
#include <string>
#include <mutex>
#include <utility> 

namespace OpenAPI {

class MyCustomApi : public OAIDefaultApi {
public:
    MyCustomApi(const int timeOut);
    ~MyCustomApi();

    std::string ping();  

private:
    bool sendCanMessage(uint32_t can_id, const uint8_t* data, size_t data_len); 
    bool receiveNextCanMessage(std::string &response);

    std::string lastPeriodicMessage;
    std::mutex lastMessageMutex;
};

} // namespace OpenAPI

#endif // MY_CUSTOM_API_H