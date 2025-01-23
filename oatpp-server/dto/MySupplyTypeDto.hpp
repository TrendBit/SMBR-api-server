#pragma once

#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * @brief Data Transfer Object representing the power supply type.
 */
class MySupplyTypeDto : public oatpp::DTO {
    DTO_INIT(MySupplyTypeDto, DTO)

    /**
     * @brief Whether the device is powered by external power supply (VIN).
     */
    DTO_FIELD(Boolean, vin);

    /**
     * @brief Whether the device is powered by Power over Ethernet (PoE).
     */
    DTO_FIELD(Boolean, poe);

    /**
     * @brief Whether the device is powered by high-budget Power over Ethernet (PoE_HB).
     */
    DTO_FIELD(Boolean, poe_hb);
};

#include OATPP_CODEGEN_END(DTO)
