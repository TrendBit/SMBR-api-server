#pragma once

#include "oatpp/Types.hpp"
#include "oatpp/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * @brief Data Transfer Object representing a module action request.
 */
class ModuleActionRequestDto : public oatpp::DTO {

  DTO_INIT(ModuleActionRequestDto, DTO)

  /**
   * @brief Unique identifier for the action request.
   */
  DTO_FIELD(String, uid); 
};

#include OATPP_CODEGEN_END(DTO)


