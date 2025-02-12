#pragma once

#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * @brief Data Transfer Object representing the serial number of the device.
 */
class SerialDto : public oatpp::DTO {
    DTO_INIT(SerialDto, DTO)

    /**
     * @brief The serial number of the device.
     */
    DTO_FIELD(Int64, serial); 
};

#include OATPP_CODEGEN_END(DTO)
