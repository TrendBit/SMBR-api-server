#pragma once

#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * @brief Data Transfer Object representing the voltage of the 5V power rail.
 */
class VoltageDto : public oatpp::DTO {
    DTO_INIT(VoltageDto, DTO)

    /**
     * @brief The voltage of the 5V power rail.
     */
    DTO_FIELD(Float32, voltage);
};

#include OATPP_CODEGEN_END(DTO)
