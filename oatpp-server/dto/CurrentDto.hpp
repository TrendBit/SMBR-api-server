#pragma once

#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * @brief Data Transfer Object representing the current consumption of the device.
 */
class CurrentDto : public oatpp::DTO {
    DTO_INIT(CurrentDto, DTO)

    /**
     * @brief The current consumption of the device.
     */
    DTO_FIELD(Float32, current);
};

#include OATPP_CODEGEN_END(DTO)
