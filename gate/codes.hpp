/**
 * @file codes.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 02.06.2024
 */

#pragma once

#include <cstdint>

/**
 * @brief   This namespace contains all enums with their respective value describing
 *              Modules, Instances, Messages and Components which are interacting over Network based on CAN Bus
 *          Intend of this header file is to be used as submodule in all repositories which needs to know this codes
 */
namespace Codes {
/**
 * @brief 8-bit identifier of device, together with 4 bit device enumeration creates, enumeration of device in system
 */
enum class Module : uint8_t {
    Undefined     = 0x00,
    All           = 0x01,
    Any           = 0x02,
    TestBed       = 0x03,
    Core_device   = 0x04,
    Control_board = 0x05,
    Sensor_board  = 0x06,
};

/**
 * @brief 4-bit identifier of instance of device, together with 8 bit device enumeration creates, enumeration of device in system
 *            Some modules can have multiple instances, some only one (this should be addressed with exclusive instance)
 */
enum class Instance : uint8_t {
    Undefined   = 0x00,
    Exclusive   = 0x01,
    All         = 0x02,
    Reserved    = 0x03,
    Instance_1  = 0x04,
    Instance_2  = 0x05,
    Instance_3  = 0x06,
    Instance_4  = 0x07,
    Instance_5  = 0x08,
    Instance_6  = 0x09,
    Instance_7  = 0x0a,
    Instance_8  = 0x0b,
    Instance_9  = 0x0c,
    Instance_10 = 0x0d,
    Instance_11 = 0x0e,
    Instance_12 = 0x0f,
};

/**
 * @brief  Identifier of abstract component of device
 *         Represent part of functionality of device
 */
enum class Component : uint32_t {
    Undefined      = 0x0000,
    Common_core    = 0x0001,
    CAN_serial     = 0x0002,
};

/**
 * @brief  11-bit identifier used by Administration messages, which are not used for application level communication
 *              But for debugging, updating or development purposes.
 *         This codes are used in non extended frame format and every non extended frame is considered as administration message
 *         Numbers of Serial_port_RX/TX extend beyond range defined here
 */
enum class Command_admin : uint16_t {
    Undefined                = 0x000,
    Serial_probe             = 0x321,
    Serial_ID_respond        = 0x323,
    Serial_port_confirmation = 0x322,
    Serial_port_RX           = 0x180,
    Serial_port_TX           = 0x181,
};

/**
 * @brief   12-bit identifier from CAN message header (ID)
 *          This codes are used in extended frame format and every extended frame is considered as application message
 *          First 4 bytes are used for message type code and also determine priority
 */

enum class Message_type : uint16_t {
    // 0x0 - Module specific - Emergency
    Undefined = 0x000,

    // 0x1 - Module specific - High priority

    // 0x2 - Common messages - High priority
    Device_reset           = 0x200,
    Device_bootloader      = 0x201,

    // 0x3 - Shared messages - High priority
    Probe_modules_request  = 0x300,
    Probe_modules_response = 0x301,

    // 0x4 - Module specific
    Supply_type_request    = 0x400,
    Supply_type_response   = 0x401,
    // 0x5 - Module specific
    // 0x6 - Module specific
    // 0x7 - Module specific
    // 0x8 - Module specific
    // 0x9 - Reserved for future use
    // 0xa - Common messages
    Core_load_request         = 0xa00,
    Core_load_response        = 0xa01,
    Core_temperature_request  = 0xa02,
    Core_temperature_response = 0xa03,
    // 0xb - Common messages
    Ping_request              = 0xb00,
    Ping_response             = 0xb01,
    // 0xc - Shared messages
    // 0xd - Reserved for future use
    // 0xe - Error messages
    // 0xf - Debugging
};
}