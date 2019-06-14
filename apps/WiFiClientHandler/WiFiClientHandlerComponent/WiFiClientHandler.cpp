/** @file WiFiClientHandler.cpp
 *
 * @brief This component handles the WiFi Network to connect to for the emc test
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2019 Current Health ltd. All rights reserved.
 */
#include "legato.h"
#include "interfaces.h"
#include <iostream>

static void MyHandleScanResult(void);
static void EventHandler(le_wifiClient_Event_t clientEvent, void *contextPtr);

static std::string wifiSSID = "WKvhnYz82BA3yTft";
static std::string wifiPSK  = "%v6eK3VTZ9DiBX=T";
le_wifiClient_NewEventHandlerRef_t wifiHandler = NULL;
static const uint8_t ssidMaxSize = 64;

static void MyHandleScanResult(void)
{
    le_wifiClient_AccessPointRef_t accessPointRef = le_wifiClient_GetFirstAccessPoint();

    while( NULL != accessPointRef )
    {
        uint8_t ssidSize = ssidMaxSize;
        uint8_t ssid[ssidMaxSize];

        le_result_t result = le_wifiClient_GetSsid( accessPointRef, ssid, (size_t*)&ssidSize);
        if (( result == LE_OK ) && ( memcmp( ssid, wifiSSID.c_str(), wifiSSID.length()) == 0 ))
        {
            LE_INFO("WiFi Client found.");

            LE_ASSERT(LE_OK == le_wifiClient_SetSecurityProtocol(accessPointRef,
                      LE_WIFICLIENT_SECURITY_WPA2_PSK_PERSONAL));

            LE_ASSERT(LE_OK == le_wifiClient_SetPassphrase(accessPointRef,
                      wifiPSK.c_str()));

            result = le_wifiClient_Connect( accessPointRef );

            if (result == LE_OK)
            {
                 LE_INFO("Connecting to AP.");
            }

            break;
        }
        accessPointRef = le_wifiClient_GetNextAccessPoint();
    }
}

static void EventHandler(le_wifiClient_Event_t clientEvent, void *contextPtr)
{
    switch( clientEvent )
    {
         case LE_WIFICLIENT_EVENT_CONNECTED:
         {
             LE_INFO("WiFi Client Connected.");
         }
         break;
         case LE_WIFICLIENT_EVENT_DISCONNECTED:
         {
             LE_INFO("WiFi Client Disconnected.");
         }
         break;
         case LE_WIFICLIENT_EVENT_SCAN_DONE:
         {
             LE_INFO("WiFi Client Scan is done.");
             MyHandleScanResult();
         }
         break;

         default:
             break;
    }
}

/*!
 * @brief Main function of the WiFiClientHandler component
 * */
COMPONENT_INIT
{
    while (1)
    {
        le_result_t result = le_wifiClient_Start();

        if ( (LE_OK == result) || (LE_BUSY == result))
        {
            LE_INFO("Bonding even handler");
            wifiHandler = le_wifiClient_AddNewEventHandler( EventHandler, NULL );
            LE_INFO("Start scan [%d]", le_wifiClient_Scan());
            break;
        }
        else
        {
            LE_INFO("ERROR: WiFi Client not started. Error %d", result);
        }

        sleep(5);
    }
}

/*** end of file ***/
