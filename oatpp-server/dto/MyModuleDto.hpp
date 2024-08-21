#ifndef MyModuleDto_hpp
#define MyModuleDto_hpp

#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class MyModuleDto : public oatpp::DTO {
  
  DTO_INIT(MyModuleDto, DTO)
  
  DTO_FIELD(String, module_type);
  DTO_FIELD(String, uid);
  
};

#include OATPP_CODEGEN_END(DTO)

#endif /* MyModuleDto_hpp */
