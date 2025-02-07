#pragma once

#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * @brief Data Transfer Object representing temperature data.
 */
class MyTempDto : public oatpp::DTO {
    DTO_INIT(MyTempDto, DTO)

    /**
     * @brief The temperature value in degrees Celsius.
     */
    DTO_FIELD(Float32, temperature); 
};

#include OATPP_CODEGEN_END(DTO)

