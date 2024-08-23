#pragma once

#include "codes/codes.hpp"

uint32_t createCanId(Codes::Message_type messageType, Codes::Module module, Codes::Instance instance, bool emergencyFlag = false);
