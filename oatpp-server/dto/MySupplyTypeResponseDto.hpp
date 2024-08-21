#ifndef MySupplyTypeResponseDto_hpp
#define MySupplyTypeResponseDto_hpp

#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class MySupplyTypeResponseDto : public oatpp::DTO {
    DTO_INIT(MySupplyTypeResponseDto, DTO)

    DTO_FIELD(Boolean, adapter); 
    DTO_FIELD(Boolean, poe);     
};

#include OATPP_CODEGEN_END(DTO)

#endif /* MySupplyTypeResponseDto_hpp */
