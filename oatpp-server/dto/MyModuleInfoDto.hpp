#ifndef MyModuleInfoDto_hpp
#define MyModuleInfoDto_hpp

#include "oatpp/Types.hpp"
#include "oatpp/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class MyModuleInfoDto : public oatpp::DTO {

  DTO_INIT(MyModuleInfoDto, DTO)

  DTO_FIELD(String, module_type); 
  DTO_FIELD(Int64, uid);          

};

#include OATPP_CODEGEN_END(DTO)

#endif /* MyModuleInfoDto_hpp */
