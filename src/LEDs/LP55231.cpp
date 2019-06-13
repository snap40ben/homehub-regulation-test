/** @file LP55231.cpp
 *
 * @brief This class provides methods to use the LP55231 LED driver
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2019 Current Health ltd. All rights reserved.
 */

#include "legato.h"
#include "interfaces.h"
#include "LEDs/LP55231.h"
#include <linux/i2c-dev.h>
#include <vector>

using namespace LP55231Constants;

/*!
 * @brief Constructor for LP55231
 *
 * @param[in] i2cInterface      Name of the i2c bus the component is on
 * @param[in] i2cAddr           Address of the LP55231 i2c slave
 *
 * */
LP55231::LP55231(std::string i2cInterface, uint8_t i2cAddr)
{
    interface = i2cInterface;
    addr = i2cAddr;
    busFile = 0;
}

/*!
 * @brief Destructor for LP55231
 *
 * */
LP55231::~LP55231()
{

}

/*!
 * @brief Init the LP55231
 *
 * @return Status of the operation.
 * */
bool LP55231::open()
{
    bool status = true;

    if ((busFile = ::open(interface.c_str(), O_RDWR)) < 0)
    {
        LE_ERROR("Failed to open the i2c bus: %s", strerror(errno));
        status = false;
    }

    if (status)
    {
        if (ioctl(busFile, I2C_SLAVE, addr) < 0)
        {
            LE_ERROR("Failed to acquire bus access and/or talk to LP55231: %s",
                                strerror(errno));
            status = false;
        }
    }

    /* Enable the LP55231 */
    writeRegister(ENABLE_ENGINE_CNTRL1, CHIP_EN);

    /* Configure the Misc register to use the internal clock and select the
     * charge pump mode automatically */
    writeRegister(MISC, CP_MODE_SELECTION_AUTO | USE_INTERNAL_CLK);

    /* Set LEDs off */
    setLEDColor(RGB_D1, 0x00, 0x00, 0x00);
    setLEDColor(RGB_D2, 0x00, 0x00, 0x00);
    setLEDColor(RGB_D3, 0x00, 0x00, 0x00);

    /* Map the 3 master faders to the 3 RGB LEDs */
    writeRegister(getLedRegister(CONTROL, RGB_D1, Red), CONTROL_MASTER_FADER1);
    writeRegister(getLedRegister(CONTROL, RGB_D1, Green), CONTROL_MASTER_FADER1);
    writeRegister(getLedRegister(CONTROL, RGB_D1, Blue), CONTROL_MASTER_FADER1);

    writeRegister(getLedRegister(CONTROL, RGB_D2, Red), CONTROL_MASTER_FADER2);
    writeRegister(getLedRegister(CONTROL, RGB_D2, Green), CONTROL_MASTER_FADER2);
    writeRegister(getLedRegister(CONTROL, RGB_D2, Blue), CONTROL_MASTER_FADER2);

    writeRegister(getLedRegister(CONTROL, RGB_D3, Red), CONTROL_MASTER_FADER3);
    writeRegister(getLedRegister(CONTROL, RGB_D3, Green), CONTROL_MASTER_FADER3);
    writeRegister(getLedRegister(CONTROL, RGB_D3, Blue), CONTROL_MASTER_FADER3);

    return status;
}


/*!
 * @brief Set one of the RGB led to the specified color
 *
 * @param[in] led       RGB Led designator
 * @param[in] red       Amount of red color to be set on 8 bits
 * @param[in] green     Amount of green color to be set on 8 bits
 * @param[in] blue      Amount of blue color to be set on 8 bits
 *
 * @return None
 * */
void LP55231::setLEDColor(RGBLED led, uint8_t red, uint8_t green, uint8_t blue)
{
    /* Set the red */
    writeRegister(getLedRegister(PWM, led, Red), red);

    /* Set the green */
    writeRegister(getLedRegister(PWM, led, Green), green);

    /* Set the blue */
    writeRegister(getLedRegister(PWM, led, Blue), blue);
}

/*!
 * @brief Write to an LP55231 register
 *
 * @param[in] registerAddr          Address of the LP55231 register to write to
 * @param[in] value                 Value to be written
 *
 * @return Status of the operation
 * */
bool LP55231::writeRegister(uint8_t registerAddr, uint8_t value)
{
    bool status = true;
    std::vector<uint8_t> buf;

    if (busFile == 0)
    {
        LE_ERROR("Please call open() successfully before using the LP55231");
        status = false;
    }

    if (status)
    {
        buf.push_back(registerAddr);
        buf.push_back(value);
        if (write(busFile, &buf[0], buf.size()) != (int)buf.size())
        {
            LE_ERROR("Failed to write to LP55231");
            status = false;
        }
    }

    return status;
}

/*!
 * @brief Read from an LP55231 register
 *
 * @param[in] registerAddr          Address of the LP55231 register to read from
 * @param[out] value                Content of the register
 *
 * @return Status of the operation
 * */
bool LP55231::readRegister(uint8_t registerAddr, uint8_t* value)
{
    bool status = true;

    if (busFile == 0)
    {
        LE_ERROR("Please call open() successfully before using the LP55231");
        status = false;
    }

    if (status)
    {
        if (write(busFile, &registerAddr, 1) != 1)
        {
            LE_ERROR("Failed to write to LP55231");
            status = false;
        }
    }

    if (status)
    {
        if (read(busFile, value, 1) != 1)
        {
            LE_ERROR("Failed to read from LP55231");
            status = false;
        }
    }

    return status;
}

/*!
 * @brief Get the register address of the LP55231 for the given LED and color
 *
 * @param[in] type          Type of register
 * @param[in] led           RGB Led
 * @param[in] color         Color of the LED to get the register for
 *
 * @return Register address
 * */
uint8_t LP55231::getLedRegister(ledRegisterType type,
                                    RGBLED led, LEDColor color)
{
    uint8_t registerBaseAddress = D1_PWM;
    uint8_t registerAddress = 0x00;

    switch(type)
    {
        case CONTROL:
            registerBaseAddress = D1_CONTROL;
            break;

        case CURRENT_CONTROL:
            registerBaseAddress = D1_CURRENT_CONTROL;
            break;

        case PWM:
            registerBaseAddress = D1_PWM;
            break;

        default:
            LE_ERROR("Register type not supported");
            break;
    }

    switch (led)
    {
        case RGB_D1:
            switch (color)
            {
                case Red:
                    registerAddress = registerBaseAddress + R1;
                    break;

                case Green:
                    registerAddress = registerBaseAddress + G1;
                    break;

                case Blue:
                    registerAddress = registerBaseAddress + B1;
                    break;

                default:
                    break;
            }
            break;

        case RGB_D2:
            switch (color)
            {
                case Red:
                    registerAddress = registerBaseAddress + R2;
                    break;

                case Green:
                    registerAddress = registerBaseAddress + G2;
                    break;

                case Blue:
                    registerAddress = registerBaseAddress + B2;
                    break;

                default:
                    break;
            }
            break;

        case RGB_D3:
            switch (color)
            {
                case Red:
                    registerAddress = registerBaseAddress + R3;
                    break;

                case Green:
                    registerAddress = registerBaseAddress + G3;
                    break;

                case Blue:
                    registerAddress = registerBaseAddress + B3;
                    break;

                default:
                    break;
            }
            break;

        default:
            break;
    }

    return registerAddress;
}

/*!
 * @brief Set the state of the selected RGB LED
 *
 * @param[in] led          RGB Led designator
 *
 * @return none
 * */
void LP55231::setLEDState(LP55231Constants::RGBLED led, RGBLedActionType type)
{
    uint8_t faderValue = 0x00;
    uint8_t faderAddr = MASTER_FADER_1;

    switch(led)
    {
        case RGB_D1:
        {
            faderAddr = MASTER_FADER_1;
            break;
        }

        case RGB_D2:
        {
            faderAddr = MASTER_FADER_2;
            break;
        }

        case RGB_D3:
        {
            faderAddr = MASTER_FADER_3;
            break;
        }

        default:
            LE_ERROR("LED not supported, D1 is used by default");
            break;
    }



    switch (type)
    {
        case TurnOn:
        {
            faderValue = 0xFF;
            break;
        }

        case Toggle:
        {
            readRegister(faderAddr, &faderValue);
            faderValue = (faderValue == 0xFF) ? 0x00 : 0xFF;
            break;
        }

        default:
            LE_ERROR("Action not supported, TurnOff is used by default");
        case TurnOff:
        {
            faderValue = 0x00;
            break;
        }
    }

    writeRegister(faderAddr, faderValue);
}

/*** end of file ***/
