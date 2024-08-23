#pragma once

#include "oatpp/Types.hpp"
#include "oatpp/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * @brief Data Transfer Object representing a module action request.
 */
class MyModuleActionRequestDto : public oatpp::DTO {

  DTO_INIT(MyModuleActionRequestDto, DTO)

  /**
   * @brief Unique identifier for the action request.
   */
  DTO_FIELD(Int64, uid); 
};

#include OATPP_CODEGEN_END(DTO)


