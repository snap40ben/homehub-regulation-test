/** @file WiFiApHandler.cpp
 *
 * @brief This component handles the WiFi Access Point used by the wearable
 * devices to transmit as well as the patient tablet to access internet.
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2019 Current Health ltd. All rights reserved.
 */
#include "legato.h"
#include "interfaces.h"
#include "WiFi/WiFiAccessPoint.h"

/*!
 * @brief Main function of the WiFiApHandler component
 * */
COMPONENT_INIT
{
    WiFiAccessPoint AccessPoint;

    AccessPoint.start();
}

/*** end of file ***/
