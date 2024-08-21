#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class MyUidDto : public oatpp::DTO {
  DTO_INIT(MyUidDto, DTO)

  DTO_FIELD(String, uid);
};

#include OATPP_CODEGEN_END(DTO)
