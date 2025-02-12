#pragma once

#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * @brief Data Transfer Object representing intensity data for all channels.
 */
class IntensitiesDto : public oatpp::DTO {
    DTO_INIT(IntensitiesDto, DTO)

    /**
     * @brief The intensities for all channels (0-3).
     */
    DTO_FIELD(Vector<Float32>::ObjectWrapper, intensity); 
};

#include OATPP_CODEGEN_END(DTO)
