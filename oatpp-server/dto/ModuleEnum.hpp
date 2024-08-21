#ifndef ModuleEnum_hpp
#define ModuleEnum_hpp

#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"

namespace dto {

#include OATPP_CODEGEN_BEGIN(DTO)

ENUM(ModuleEnum, v_int32,
     VALUE(core, 0, "core"),
     VALUE(control, 1, "control"),
     VALUE(sensor, 2, "sensor"))

#include OATPP_CODEGEN_END(DTO)

}

#endif /* ModuleEnum_hpp */