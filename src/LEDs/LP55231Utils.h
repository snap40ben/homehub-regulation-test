/** @file LP55231Utils.h
 *
 * @brief This file is used to define constants for the LP55231
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2019 Current Health ltd. All rights reserved.
 */

#ifndef LP55231_UTILS_H
#define LP55231_UTILS_H

#include "legato.h"
#include "interfaces.h"
#include <iostream>
#include <vector>

namespace LP55231Constants
{
    enum Registers
    {
        ENABLE_ENGINE_CNTRL1            = 0x00,
        ENGINE_CNTRL2                   = 0x01,

        OUTPUT_DIRECT_RAT_MSB           = 0x02,
        OUTPUT_DIRECT_RAT_LSB           = 0x03,

        OUTPUT_ONOFF_CTRL_MSB           = 0x04,
        OUTPUT_ONOFF_CTRL_LSB           = 0x05,

        D1_CONTROL                      = 0x06,
        D2_CONTROL                      = 0x07,
        D3_CONTROL                      = 0x08,
        D4_CONTROL                      = 0x09,
        D5_CONTROL                      = 0x0A,
        D6_CONTROL                      = 0x0B,
        D7_CONTROL                      = 0x0C,
        D8_CONTROL                      = 0x0D,
        D9_CONTROL                      = 0x0E,

        D1_PWM                          = 0x16,
        D2_PWM                          = 0x17,
        D3_PWM                          = 0x18,
        D4_PWM                          = 0x19,
        D5_PWM                          = 0x1A,
        D6_PWM                          = 0x1B,
        D7_PWM                          = 0x1C,
        D8_PWM                          = 0x1D,
        D9_PWM                          = 0x1E,

        D1_CURRENT_CONTROL              = 0x26,
        D2_CURRENT_CONTROL              = 0x27,
        D3_CURRENT_CONTROL              = 0x28,
        D4_CURRENT_CONTROL              = 0x29,
        D5_CURRENT_CONTROL              = 0x2A,
        D6_CURRENT_CONTROL              = 0x2B,
        D7_CURRENT_CONTROL              = 0x2C,
        D8_CURRENT_CONTROL              = 0x2D,
        D9_CURRENT_CONTROL              = 0x2E,

        MISC                            = 0x36,

        ENGINE_1_PC                     = 0x37,
        ENGINE_2_PC                     = 0x38,
        ENGINE_3_PC                     = 0x39,

        STATUS_INT                      = 0x3A,
        INT_GPIO                        = 0x3B,
        VARIABLE                        = 0x3C,
        RESET                           = 0x3D,
        TEMP_ADC_CONTROL                = 0x3E,
        TEMPERATURE_READ                = 0x3F,
        TEMPERATURE_WRITE               = 0x40,
        LED_TEST_CONTROL                = 0x41,
        LED_TEST_ADC                    = 0x42,

        ENGINE_1_VAR_A                  = 0x45,
        ENGINE_2_VAR_A                  = 0x46,
        ENGINE_3_VAR_A                  = 0x47,

        MASTER_FADER_1                  = 0x48,
        MASTER_FADER_2                  = 0x49,
        MASTER_FADER_3                  = 0x4A,

        ENG1_PROG_START_ADDR            = 0x4C,
        ENG2_PROG_START_ADDR            = 0x4D,
        ENG3_PROG_START_ADDR            = 0x4E,

        PROG_MEM_PAGE_SEL               = 0x4E,

        PROGRAM_MEMORY_BASE_ADDR        = 0x50,

        ENG1_MAPPING_MSB                = 0x70,
        ENG1_MAPPING_LSB                = 0x71,
        ENG2_MAPPING_MSB                = 0x72,
        ENG2_MAPPING_LSB                = 0x73,
        ENG3_MAPPING_MSB                = 0x74,
        ENG3_MAPPING_LSB                = 0x75,

        GAIN_CHANGE_CTRL                = 0x76,
    };

    enum RGBLED
    {
        RGB_D1,
        RGB_D2,
        RGB_D3,
    };

    enum LEDColor
    {
        Red,
        Green,
        Blue,
    };

    enum LP55231LedList
    {
        D1 = 0,
        D2,
        D3,
        D4,
        D5,
        D6,
        D7,
        D8,
        D9,
    };

    enum ledRegisterType
    {
        CONTROL,
        CURRENT_CONTROL,
        PWM
    };

    const uint8_t CHIP_EN                   = (1 << 6);
    const uint8_t CP_MODE_SELECTION_AUTO    = (1 << 4) | (1 << 3);
    const uint8_t USE_INTERNAL_CLK          = (1 << 1) | (1 << 0);

    const uint8_t CONTROL_MASTER_FADER1     = (1 << 6);
    const uint8_t CONTROL_MASTER_FADER2     = (1 << 7);
    const uint8_t CONTROL_MASTER_FADER3     = (1 << 6) | (1 << 7);

    /* Mapping between board LEDs and LP55231 outputs */
    const uint8_t R1 = D7;
    const uint8_t R2 = D8;
    const uint8_t R3 = D9;
    const uint8_t G1 = D1;
    const uint8_t G2 = D3;
    const uint8_t G3 = D5;
    const uint8_t B1 = D2;
    const uint8_t B2 = D4;
    const uint8_t B3 = D6;

    enum RGBLedActionType
    {
        TurnOn,
        TurnOff,
        Toggle
    };
}

#endif /* LP55231_UTILS_H */

/*** end of file ***/
