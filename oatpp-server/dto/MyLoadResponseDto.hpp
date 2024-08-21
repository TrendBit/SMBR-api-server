#ifndef MyLoadResponseDto_hpp
#define MyLoadResponseDto_hpp

#include "oatpp/Types.hpp"
#include "oatpp/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class MyLoadResponseDto : public oatpp::DTO {
  
  DTO_INIT(MyLoadResponseDto, DTO)

  DTO_FIELD(Float32, load); 
  DTO_FIELD(Int32, cores); 
  
};

#include OATPP_CODEGEN_END(DTO)

#endif /* MyLoadResponseDto_hpp */
