/** @file CellularNetworkUtils.h
 *
 * @brief This file is used to define constants for the Cellular Network
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2019 Current Health ltd. All rights reserved.
 */

#ifndef CELLULAR_NETWORK_UTILS_H
#define CELLULAR_NETWORK_UTILS_H

#include "legato.h"
#include "interfaces.h"
#include <iostream>
#include <vector>

namespace CellularNetworkConstants
{
    /* Twilio profile settings */
    const uint8_t TWILIO_PROFILE_INDEX = 1;
    const le_mdc_Pdp_t TWILIO_PDP = LE_MDC_PDP_IPV4;
    const std::string TWILIO_APN  = "wireless.twilio.com";

    /* This vector defines a number of seconds to wait a each stages of the
     * reconnect process. The point is to avoid looping in a 5 sec reconnect
     * process and allow for an OTA to happen if the reconnect process is
     * returning a false negative reason. */
    const std::vector<uint8_t> RECONNECT_SLEEPTIMES =
    {
        /* Stage 0 */
        5,
        /* Stage 1 */
        15,
        /* Stage 2 */
        180
    };

    /* AirVantage Management Services Config */

    /* Accept connection to Airvantage service */
    const std::string  AVMS_AUTO_AGREEMENT  = "at+wdsc=0,0";

    /* Accept updates download from Airvantage service */
    const std::string  PCKG_DL_AUTO_AGREEMENT   = "at+wdsc=1,0";

    /* Accept update install from Airvantage service */
    const std::string  PCKG_INSTALL_AUTO_AGREEMENT  = "at+wdsc=2,0";

    /* Airvantage polling period */
    const std::string  POLLING_PERIOD_MIN = "15";
    const std::string  POLLING_MODE = "at+wdsc=3," + POLLING_PERIOD_MIN;

    /*  Accept reboot commands from Airvantage service */
    const std::string  REBOOT_AUTO_AGREEMENT = "at+wdsc=5,0";

    /* Accept package uninstall command from Airvantage service */
    const std::string  PCKG_UNINSTALL_AUTO_AGREEMENT = "at+wdsc=6,0";

    /* AirVantage Service Session */
    /* Set a one time connection */
    const std::string  CONNECT_TO_AIRVANTAGE = "at+wdss=1,1";

    /* AT commands to be done when internet connection is done */
    const std::vector<std::string> AT_STARTUP_CMDS = {
            AVMS_AUTO_AGREEMENT,
            PCKG_DL_AUTO_AGREEMENT,
            PCKG_INSTALL_AUTO_AGREEMENT,
            POLLING_MODE,
            REBOOT_AUTO_AGREEMENT,
            PCKG_UNINSTALL_AUTO_AGREEMENT,
            CONNECT_TO_AIRVANTAGE
    };

    /* Period between two connectivity check aka heartbeats */
    const uint16_t HEARTBEAT_PERIOD_S = 600;
}

#endif /* CELLULAR_NETWORK_UTILS_H */

/*** end of file ***/
