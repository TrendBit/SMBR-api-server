#ifndef MyModuleActionRequestDto_hpp
#define MyModuleActionRequestDto_hpp

#include "oatpp/Types.hpp"
#include "oatpp/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class MyModuleActionRequestDto : public oatpp::DTO {

  DTO_INIT(MyModuleActionRequestDto, DTO)

  DTO_FIELD(Int64, uid); 
};

#include OATPP_CODEGEN_END(DTO)

#endif /* MyModuleActionRequestDto_hpp */
