#ifndef PingResponseDto_hpp
#define PingResponseDto_hpp

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class PingResponseDto : public oatpp::DTO {
  
  DTO_INIT(PingResponseDto, DTO)
  
  DTO_FIELD(Float32, time_ms);  
  
};

#include OATPP_CODEGEN_END(DTO)

#endif /* PingResponseDto_hpp */
