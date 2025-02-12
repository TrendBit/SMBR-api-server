#pragma once

#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * @brief Data Transfer Object representing the Short ID (SID).
 */
class SIDDto : public oatpp::DTO {
    DTO_INIT(SIDDto, DTO)

    /**
     * @brief The Short ID (SID) of the device.
     */
    DTO_FIELD(String, sid); 
};

#include OATPP_CODEGEN_END(DTO)
