#pragma once

#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * @brief Data Transfer Object representing the voltage of the 5V power rail.
 */
class MyVoltageDto : public oatpp::DTO {
    DTO_INIT(MyVoltageDto, DTO)

    /**
     * @brief The voltage of the 5V power rail.
     */
    DTO_FIELD(Float32, voltage);
};

#include OATPP_CODEGEN_END(DTO)
