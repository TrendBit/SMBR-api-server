#ifndef CANIDGENERATOR_HPP
#define CANIDGENERATOR_HPP

#include "codes/codes.hpp"

uint32_t createCanId(Codes::Message_type messageType, Codes::Module module, Codes::Instance instance, bool emergencyFlag);

#endif // CANIDGENERATOR_HPP
