#pragma once

#include "oatpp/Types.hpp"
#include "oatpp/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * @brief Data Transfer Object containing module information.
 */
class MyModuleInfoDto : public oatpp::DTO {

  DTO_INIT(MyModuleInfoDto, DTO)

  /**
   * @brief The type of the module.
   */
  DTO_FIELD(String, module_type); 

  /**
   * @brief Unique identifier for the module.
   */
  DTO_FIELD(Int64, uid);          

};

#include OATPP_CODEGEN_END(DTO)


