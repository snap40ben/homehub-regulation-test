/** @file LEDControllerUtils.h
 *
 * @brief This file is used to define constants and types for the LEDController
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2019 Current Health ltd. All rights reserved.
 */

#ifndef LED_CONTROLLER_UTILS_H
#define LED_CONTROLLER_UTILS_H

#include "legato.h"
#include "interfaces.h"
#include "LEDs/LP55231Utils.h"
#include <iostream>

namespace LEDControllerTypes
{
    enum ledCommand
    {
        On,
        Off,
        Blink
    };
}

#endif /* LED_CONTROLLER_UTILS_H */

/*** end of file ***/
