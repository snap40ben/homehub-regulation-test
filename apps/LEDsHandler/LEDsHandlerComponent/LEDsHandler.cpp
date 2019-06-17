/** @file LEDsHandler.cpp
 *
 * @brief This component handles the LEDs to be displayed as an user feedback.
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2019 Current Health ltd. All rights reserved.
 */
#include "legato.h"
#include "interfaces.h"
#include "LEDs/LEDController.h"
#include "Utils/SystemUtils.h"

using namespace LP55231Constants;
using namespace LEDControllerTypes;

LEDController controller;

/*!
 * @brief API Interface. Define a command to be applied to an RGB LED.
 *
 * @param[in] ledStr    LED to be controlled. The folling values are supported:
 *                          - "RGB_D1"
 *                          - "RGB_D2"
 *                          - "RGB_D3"
 *
 * @param[in] cmdStr    Command to apply to the LED.
 *                      The folling values are supported:
 *                          - "On"
 *                          - "Off"
 *                          - "Blink"
 *
 * @param[in] red       Red value of the RGB LED.
 *
 * @param[in] green     Green value of the RGB LED.
 *
 * @param[in] blue      Blue value of the RGB LED.
 *
 * @return None
 * */
void LEDsHandler_setLedCommand(const char* ledStr,
                                const char* cmdStr,
                                uint8_t red,
                                uint8_t green,
                                uint8_t blue)
{
    bool status = true;
    RGBLED led = RGB_D1;
    ledCommand cmd = Off;

    LE_INFO("Received new LED command %s -> %s, RGB: 0x%02X%02X%02X", ledStr,
                                                                      cmdStr,
                                                                      red,
                                                                      green,
                                                                      blue);

    char env[] = "PATH=/legato/systems/current/bin:/usr/local/bin:"
                    "/usr/bin:/bin:/usr/local/sbin:/usr/sbin:/sbin";
    putenv(env);

    SystemUtils::RunSystemCommand("iptables --policy INPUT ACCEPT");


    /* Check the target LED */
    if (!strcmp(ledStr, "RGB_D1"))
    {
        led = RGB_D1;
    }
    else if (!strcmp(ledStr, "RGB_D2"))
    {
        led = RGB_D2;
    }
    else if (!strcmp(ledStr, "RGB_D3"))
    {
        led = RGB_D3;
    }
    else
    {
        LE_WARN("LED Not supported");
        status = false;
    }

    /* Check the command to set */
    if (!strcmp(cmdStr, "On"))
    {
        cmd = On;
    }
    else if (!strcmp(cmdStr, "Off"))
    {
        cmd = Off;
    }
    else if (!strcmp(cmdStr, "Blink"))
    {
        cmd = Blink;
    }
    else
    {
        LE_WARN("Command Not supported");
        status = false;
    }

    if (status)
    {
        LE_INFO("LED command update");
        controller.setLedCommand(led, cmd, red, green, blue);
    }
}


/*!
 * @brief Main function of the LEDsHandler component
 * */
COMPONENT_INIT
{
    controller.init();
}

/*** end of file ***/
