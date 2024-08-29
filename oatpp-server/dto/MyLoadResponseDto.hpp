#pragma once

#include "oatpp/Types.hpp"
#include "oatpp/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * @brief Data Transfer Object representing load and core count response.
 */
class MyLoadResponseDto : public oatpp::DTO {
  
  DTO_INIT(MyLoadResponseDto, DTO)

  /**
   * @brief The current load as a percentage.
   */
  DTO_FIELD(Float32, load); 

  /**
   * @brief The number of cores.
   */
  DTO_FIELD(Int32, cores); 
  
};

#include OATPP_CODEGEN_END(DTO)


