#ifndef MyTempDto_hpp
#define MyTempDto_hpp

#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class MyTempDto : public oatpp::DTO {
    DTO_INIT(MyTempDto, DTO)

    DTO_FIELD(Float32, temperature); 
};

#include OATPP_CODEGEN_END(DTO)

#endif /* MyTempDto_hpp */
