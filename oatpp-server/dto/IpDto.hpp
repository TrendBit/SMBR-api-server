#pragma once

#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * @brief Data Transfer Object representing the IP address.
 */
class IpDto : public oatpp::DTO {
    DTO_INIT(IpDto, DTO)

    /**
     * @brief The IP address of the device.
     */
    DTO_FIELD(String, ipAddress); 
};

#include OATPP_CODEGEN_END(DTO)
