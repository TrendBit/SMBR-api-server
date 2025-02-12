#pragma once

#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * @brief Data Transfer Object representing flowrate data.
 */
class FlowrateDto : public oatpp::DTO {
    DTO_INIT(FlowrateDto, DTO)

    /**
     * @brief The flowrate value in ml/min.
     */
    DTO_FIELD(Float32, flowrate); 
};

#include OATPP_CODEGEN_END(DTO)
