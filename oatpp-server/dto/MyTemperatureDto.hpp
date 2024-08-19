#ifndef MyTemperatureDto_hpp
#define MyTemperatureDto_hpp

#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class MyTemperatureDto : public oatpp::DTO {
  
  DTO_INIT(MyTemperatureDto, DTO)
  
  DTO_FIELD(Int32, statusCode);
  DTO_FIELD(Float32, temperature);
  
};

#include OATPP_CODEGEN_END(DTO)

#endif 
