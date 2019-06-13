/** @file LEDController.h
 *
 * @brief This class provides methods to manage the LEDs and the timer used to
 * make them blink
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2019 Current Health ltd. All rights reserved.
 */

#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include "legato.h"
#include "interfaces.h"
#include "LEDs/LP55231.h"
#include "LEDs/LEDControllerUtils.h"
#include <iostream>

class LEDController
{
    private:
        static void ledTimerHandler(le_timer_Ref_t ledTimerRef);
        static LP55231 LedDriver;
        le_timer_Ref_t ledTimer;
        static LEDControllerTypes::ledCommand d1Command, d2Command, d3Command;

    public:
        LEDController();
        ~LEDController();
        bool init();
        static void updateLed(LP55231Constants::RGBLED led);
        void setLedCommand(LP55231Constants::RGBLED led,
                            LEDControllerTypes::ledCommand command,
                            uint8_t red, uint8_t green, uint8_t blue);
};

#endif /* LED_CONTROLLER_H */

/*** end of file ***/
