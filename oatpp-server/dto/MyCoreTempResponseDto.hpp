#ifndef MyCoreTempResponseDto_hpp
#define MyCoreTempResponseDto_hpp

#include "oatpp/Types.hpp"
#include "oatpp/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class MyCoreTempResponseDto : public oatpp::DTO {
  
  DTO_INIT(MyCoreTempResponseDto, DTO)

  DTO_FIELD(Float32, core_temp); 
  
};

#include OATPP_CODEGEN_END(DTO)

#endif /* MyCoreTempResponseDto_hpp */
