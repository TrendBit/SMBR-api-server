#pragma once

#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * @brief Data Transfer Object representing intensity data.
 */
class MyIntensityDto : public oatpp::DTO {
    DTO_INIT(MyIntensityDto, DTO)

    /**
     * @brief The intensity value between 0 and 1.
     */
    DTO_FIELD(Float32, intensity); 

};

#include OATPP_CODEGEN_END(DTO)