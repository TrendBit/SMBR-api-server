#pragma once

#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * @brief Data Transfer Object representing a ping response.
 */
class MyPingResponseDto : public oatpp::DTO {
  
  DTO_INIT(MyPingResponseDto, DTO)
  
  /**
   * @brief Time taken for the ping response in milliseconds.
   */
  DTO_FIELD(Float32, time_ms);  
  
};

#include OATPP_CODEGEN_END(DTO)


