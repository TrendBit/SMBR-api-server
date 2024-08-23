#pragma once

#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * @brief Data Transfer Object representing the supply type response.
 */
class MySupplyTypeResponseDto : public oatpp::DTO {
    DTO_INIT(MySupplyTypeResponseDto, DTO)

    /**
     * @brief Indicates if the supply type is an adapter.
     */
    DTO_FIELD(Boolean, adapter); 

    /**
     * @brief Indicates if the supply type is PoE (Power over Ethernet).
     */
    DTO_FIELD(Boolean, poe);     
};

#include OATPP_CODEGEN_END(DTO)


