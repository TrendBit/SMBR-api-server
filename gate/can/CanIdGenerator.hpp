/**
 * @file CanIdGenerator.hpp
 * @version 0.1
 * @date 23.08.2024
 */

#pragma once

#include "codes/codes.hpp"

/**
 * @brief Generates a CAN ID based on the provided message type, module, instance, and emergency flag.
 * @param messageType The type of the CAN message.
 * @param module The module identifier.
 * @param instance The instance identifier.
 * @param emergencyFlag Whether the message is an emergency.
 * @return The generated CAN ID.
 */
uint32_t createCanId(Codes::Message_type messageType, Codes::Module module, Codes::Instance instance, bool emergencyFlag);


