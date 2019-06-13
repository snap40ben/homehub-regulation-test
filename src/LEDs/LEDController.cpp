/** @file LEDController.cpp
 *
 * @brief This class provides methods to manage the LEDs and the timer used to
 * make them blink
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2019 Current Health ltd. All rights reserved.
 */

#include "legato.h"
#include "interfaces.h"
#include "LEDs/LEDController.h"

using namespace LP55231Constants;
using namespace LEDControllerTypes;

static const uint32_t LED_BLINKING_INTERVAL_MS = 500;
static const uint8_t LP55231_SLAVE_ADDR = 0x32;
static const std::string I2C_INTERFACE = "/dev/i2c-4";

LP55231 LEDController::LedDriver(I2C_INTERFACE.c_str(), LP55231_SLAVE_ADDR);
ledCommand LEDController::d1Command = Off;
ledCommand LEDController::d2Command = Off;
ledCommand LEDController::d3Command = Off;

/*!
 * @brief Constructor for LEDController
 * */
LEDController::LEDController()
{

}

/*!
 * @brief Destructor for LEDController
 *
 * */
LEDController::~LEDController()
{

}

/*!
 * @brief Init the LEDController
 *
 * @return Status of the operation
 * */
bool LEDController::init()
{
    bool status = true;

    /* Create the blinking timer */
    ledTimer = le_timer_Create("ledTimer");

    /* Set it to repeat indefinitely */
    le_timer_SetRepeat(ledTimer, 0);

    /* Configure interval, handler and start it*/
    le_timer_SetMsInterval(ledTimer, LED_BLINKING_INTERVAL_MS);
    le_timer_SetHandler(ledTimer, ledTimerHandler);
    le_timer_Start(ledTimer);

    status = LedDriver.open();

    return status;
}

/*!
 * @brief Refresh LED state according to configuration
 *
 * @param[in] ledTimerRef Reference to the timer
 *
 * @return None
 * */
void LEDController::ledTimerHandler(le_timer_Ref_t ledTimerRef)
{
    updateLed(RGB_D1);
    updateLed(RGB_D2);
    updateLed(RGB_D3);
}

/*!
 * @brief Update the LED according to the sommand defined with setLedCommand
 *
 * @param[in] led       LED designator
 *
 * @return None
 * */
void LEDController::updateLed(RGBLED led)
{
    RGBLedActionType type = TurnOff;
    ledCommand command = d1Command;

    switch (led)
    {
        case RGB_D1:
            command = d1Command;
            break;

        case RGB_D2:
            command = d2Command;
            break;

        case RGB_D3:
            command = d3Command;
            break;

        default:
            LE_ERROR("LED not supported, D1 used by default");
            break;
    }

    if (command == Blink)
    {
        type = Toggle;
    }
    else if (command == On)
    {
        type = TurnOn;
    }

    LedDriver.setLEDState(led, type);
}

/*!
 * @brief Update the LED command to be used with an LED
 *
 * @param[in] led       LED designator
 * @param[in] red       Red value to configure
 * @param[in] green     Green value to configure
 * @param[in] blue      Blue value to configure
 * @param[in] command   Command to use on the LED
 *
 * @return None
 * */
void LEDController::setLedCommand(RGBLED led, ledCommand command, uint8_t red,
                                    uint8_t green, uint8_t blue)
{
    switch (led)
    {
        case RGB_D1:
            d1Command = command;
            break;

        case RGB_D2:
            d2Command = command;
            break;

        case RGB_D3:
            d3Command = command;
            break;

        default:
            LE_ERROR("LED not supported");
            break;
    }

    LedDriver.setLEDColor(led, red, green, blue);
}


/*** end of file ***/
