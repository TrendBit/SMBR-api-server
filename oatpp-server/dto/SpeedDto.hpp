#pragma once

#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * @brief Data Transfer Object representing pump speed data.
 */
class SpeedDto : public oatpp::DTO {
    DTO_INIT(SpeedDto, DTO)

    /**
     * @brief The speed value between -1.0 and 1.0.
     */
    DTO_FIELD(Float32, speed); 
};

#include OATPP_CODEGEN_END(DTO)
