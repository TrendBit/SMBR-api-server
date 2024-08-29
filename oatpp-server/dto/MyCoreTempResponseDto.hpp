#pragma once

#include "oatpp/Types.hpp"
#include "oatpp/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * @brief Data Transfer Object representing core temperature response.
 */
class MyCoreTempResponseDto : public oatpp::DTO {
  
  DTO_INIT(MyCoreTempResponseDto, DTO)

  /**
   * @brief Core temperature in degrees Celsius.
   */
  DTO_FIELD(Float32, core_temp); 
  
};

#include OATPP_CODEGEN_END(DTO)


