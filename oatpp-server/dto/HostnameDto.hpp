#pragma once

#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * @brief Data Transfer Object representing the hostname of the device.
 */
class MyHostnameDto : public oatpp::DTO {
    DTO_INIT(MyHostnameDto, DTO)

    /**
     * @brief The hostname of the device, truncated to 8 characters.
     */
    DTO_FIELD(String, hostname); 
};

#include OATPP_CODEGEN_END(DTO)
