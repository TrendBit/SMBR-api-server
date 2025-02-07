#pragma once

#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * @brief Data Transfer Object representing mixer RPM data.
 */
class MyRpmDto : public oatpp::DTO {
    DTO_INIT(MyRpmDto, DTO)

    /**
     * @brief The RPM value, expected to be between 0 and 10000.
     */
    DTO_FIELD(Float32, rpm); 
};

#include OATPP_CODEGEN_END(DTO)
