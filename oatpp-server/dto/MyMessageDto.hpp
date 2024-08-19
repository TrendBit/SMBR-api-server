#ifndef MyMessageDto_hpp
#define MyMessageDto_hpp

#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class MyMessageDto : public oatpp::DTO {
  
  DTO_INIT(MyMessageDto, DTO)
  
  DTO_FIELD(Int32, statusCode);
  DTO_FIELD(String, message);
  
};

#include OATPP_CODEGEN_END(DTO)

#endif 
