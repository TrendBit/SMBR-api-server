#pragma once

#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * @brief Error Response Data Transfer Object.
 */
class ErrorResponseDto : public oatpp::DTO {

  DTO_INIT(ErrorResponseDto, DTO)

  /**
   * @brief Error message.
   */
  DTO_FIELD(String, message);
};

#include OATPP_CODEGEN_END(DTO)
