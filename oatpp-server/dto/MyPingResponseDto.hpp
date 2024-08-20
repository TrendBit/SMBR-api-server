#ifndef MyPingResponseDto_hpp
#define MyPingResponseDto_hpp

#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class MyPingResponseDto : public oatpp::DTO {
  
  DTO_INIT(MyPingResponseDto, DTO)
  
  DTO_FIELD(Float32, time_ms);  
  
};

#include OATPP_CODEGEN_END(DTO)

#endif /* MyPingResponseDto_hpp */
