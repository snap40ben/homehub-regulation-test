/** @file WiFiAccessPoint.cpp
 *
 * @brief This class is used to handle the WiFi Access Point provided by the
 * homehub
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2019 Current Health ltd. All rights reserved.
 */

#include "legato.h"
#include "interfaces.h"
#include "Wifi/WiFiAccessPoint.h"
#include "WiFi/WiFiAccessPointUtils.h"
#include "Utils/SystemUtils.h"
#include <signal.h>

using namespace WiFiAccessPointConstants;

/*!
 * @brief Constructor for WiFiAccessPoint
 *
 * */
WiFiAccessPoint::WiFiAccessPoint()
{
    char env[] = "PATH=/legato/systems/current/bin:/usr/local/bin:"
            "/usr/bin:/bin:/usr/local/sbin:/usr/sbin:/sbin";
    putenv(env);

    WiFiAccessPoint::stop();
}

/*!
 * @brief Destructor for WiFiAccessPoint
 *
 * */
WiFiAccessPoint::~WiFiAccessPoint()
{

}

/*!
 * @brief Configure the DHCP Server and the bridge used to forward internet
 *
 * @return None
 */
void WiFiAccessPoint::configureDHCPAndIpTables(void)
{
    /* Flush the IP address of the wlan0 interface */
    SystemUtils::RunSystemCommand("ip addr flush dev " + ITF_LAN);

    SystemUtils::RunSystemCommand("ifconfig " + ITF_LAN +
                                " " + HOST_IP + " up");

    SystemUtils::RunSystemCommand("iptables -I INPUT -s " + SUBNET +
                                " --jump ACCEPT");

    SystemUtils::RunSystemCommand("iptables --policy INPUT ACCEPT");

    LE_ASSERT(LE_OK == le_wifiAp_SetIpRange(HOST_IP.c_str(),
                                                IP_RANGE_START.c_str(),
                                                IP_RANGE_END.c_str()));

    /* Configure the IP aliasing */
    /*SystemUtils::RunSystemCommand("ifconfig " + ITF_LAN + ":" +
                                    TARGET_IP_ALIAS_NUMBER + " " + TARGET_IP);*/
}

/*!
 * @brief Configure the access point parameters.
 *
 * @return None
 */
void WiFiAccessPoint::setApParameters(void)
{
    LE_ASSERT(LE_OK == le_wifiAp_SetSsid(WIFI_AP_SSID, sizeof(WIFI_AP_SSID)));

    LE_ASSERT(LE_OK == le_wifiAp_SetPassPhrase(WIFI_AP_PASSPHRASE));

    LE_ASSERT(LE_OK == le_wifiAp_SetSecurityProtocol(WIFI_AP_SECURITY));

    LE_ASSERT(LE_OK == le_wifiAp_SetDiscoverable(true));
}

/*!
 * @brief Start the WiFi access point
 *
 * @return None
 */
void WiFiAccessPoint::start(void)
{
    LE_INFO("Start Test WiFi access point");

    WiFiAccessPoint::setApParameters();

    while (le_wifiAp_Start() != LE_OK)
    {
        LE_ERROR("Start ERROR, retry in %d seconds", WIFI_AP_RETRY_SEC);

        sleep(WIFI_AP_RETRY_SEC);

        this->stop();

        WiFiAccessPoint::setApParameters();
    }

    WiFiAccessPoint::configureDHCPAndIpTables();
}

/*!
 * @brief Stop the WiFi access point
 *
 * @return None
 */
void WiFiAccessPoint::stop(void)
{
    /* Stop the AP */
    le_wifiAp_Stop();

    /* Run sierra script manually to workaround a bug in the start script. */
    /* Problem in start is that if at least one wifi module were already
     * started, the script will consider everything is configured.
     * The problem will lead to having not all the modules started + the GPIO
     * not configured. */
    SystemUtils::RunSystemCommand("/etc/init.d/tiwifi stop");
}

/*** end of file ***/
