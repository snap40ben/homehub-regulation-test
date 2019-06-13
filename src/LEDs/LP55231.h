/** @file LP55231.h
 *
 * @brief This class provides methods to use the LP55231 LED driver
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2019 Current Health ltd. All rights reserved.
 */

#ifndef LP55231_H
#define LP55231_H

#include "legato.h"
#include "interfaces.h"
#include "LEDs/LP55231Utils.h"
#include <iostream>

class LP55231
{
    private:
        std::string interface;
        uint8_t addr;
        int32_t busFile;
        bool writeRegister(uint8_t registerAddr, uint8_t value);
        bool readRegister(uint8_t registerAddr, uint8_t* value);
        uint8_t getLedRegister(LP55231Constants::ledRegisterType type,
                                LP55231Constants::RGBLED led,
                                LP55231Constants::LEDColor color);

    public:
        LP55231(std::string i2cInterface, uint8_t i2cAddr);
        ~LP55231();
        bool open();
        void setLEDColor(LP55231Constants::RGBLED led, uint8_t red,
                            uint8_t green, uint8_t blue);
        void setLEDState(LP55231Constants::RGBLED led,
                            LP55231Constants::RGBLedActionType type);
};

#endif /* LP55231_H */

/*** end of file ***/
