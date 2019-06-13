/** @file WiFiAccessPointUtils.h
 *
 * @brief This file is used to define constants for the WiFi Access Point
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2019 Current Health ltd. All rights reserved.
 */

#ifndef WIFI_ACCESS_POINT_UTILS_H
#define WIFI_ACCESS_POINT_UTILS_H

#include "legato.h"
#include "interfaces.h"
#include <iostream>

namespace WiFiAccessPointConstants
{
    /* WiFi AP Credential and security */
    const uint8_t WIFI_AP_SSID[] = "HomehubEmcTest";
    const char WIFI_AP_PASSPHRASE[] = "Snap40Snap40";
    const le_wifiAp_SecurityProtocol_t WIFI_AP_SECURITY =
                                                    LE_WIFIAP_SECURITY_WPA2;

    /* IP & mask of subnet created on the wlan */
    const std::string SUBNET  = "192.168.10.0/24";
    const std::string HOST_IP = "192.168.10.1";

    /* IP range allotted to clients */
    const std::string IP_RANGE_START = "192.168.10.10";
    const std::string IP_RANGE_END =   "192.168.10.240";

    /* Interface of the access point (LAN - Local Area Network) */
    const std::string ITF_LAN = "wlan0";

    /* Sleep duration before WiFi Ap start retry */
    const uint8_t WIFI_AP_RETRY_SEC = 5;

    /* IP Aliasing */
    const std::string TARGET_IP = "192.168.10.2";
    const std::string TARGET_IP_ALIAS_NUMBER = "0";
}

#endif /* WIFI_ACCESS_POINT_UTILS_H */

/*** end of file ***/
