#pragma once

#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * @brief Data Transfer Object representing volume and flowrate data.
 */
class MyMoveDto : public oatpp::DTO {
    DTO_INIT(MyMoveDto, DTO)

    DTO_FIELD(Float32, volume, "volume"); 
    DTO_FIELD(Float32, flowrate, "flowrate"); 
};


#include OATPP_CODEGEN_END(DTO)
