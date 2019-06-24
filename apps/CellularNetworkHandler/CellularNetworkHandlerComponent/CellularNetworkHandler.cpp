/** @file CellularNetworkHandler.cpp
 *
 * @brief This component handles the Cellular network used by the homehub to
 * transmit data to the Current Health server.
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2019 Current Health ltd. All rights reserved.
 */
#include "legato.h"
#include "interfaces.h"
#include "CellularNetwork/CellularNetwork.h"
#include "CellularNetwork/CellularNetworkUtils.h"
#include "Utils/SystemUtils.h"
#include "Utils/TimeUpdater.h"

using namespace CellularNetworkConstants;

static const std::string CONNECTIVITY_LED_NAME              = "RGB_D2";
static const std::string CONNECTIVITY_LED_CMD_CONNECTED     = "On";
static const std::string CONNECTIVITY_LED_CMD_NOT_CONNECTED = "Blink";
static const uint8_t CONNECTIVITY_LED_RED                   = 0x03;
static const uint8_t CONNECTIVITY_LED_GREEN                 = 0x00;
static const uint8_t CONNECTIVITY_LED_BLUE                  = 0x36;

/*!
 * @brief Main function of the CellularNetworkHandler component. Start and
 * maintain network connectivity.
 *
 * */
COMPONENT_INIT
{
    CellularNetwork cellNetwork;
    uint8_t reconnectStage = 0;
    bool isConnected = false;
    char env[] = "PATH=/legato/systems/current/bin:/usr/local/bin:"
                "/usr/bin:/bin:/usr/local/sbin:/usr/sbin:/sbin";
    putenv(env);

    LEDsHandler_setLedCommand(CONNECTIVITY_LED_NAME.c_str(),
                                CONNECTIVITY_LED_CMD_NOT_CONNECTED.c_str(),
                                CONNECTIVITY_LED_RED,
                                CONNECTIVITY_LED_GREEN,
                                CONNECTIVITY_LED_BLUE);

    cellNetwork.open();

    while(1)
    {
        isConnected = cellNetwork.checkConnectivity();

        while(!isConnected)
        {
            LEDsHandler_setLedCommand(CONNECTIVITY_LED_NAME.c_str(),
                                    CONNECTIVITY_LED_CMD_NOT_CONNECTED.c_str(),
                                    CONNECTIVITY_LED_RED,
                                    CONNECTIVITY_LED_GREEN,
                                    CONNECTIVITY_LED_BLUE);


            cellNetwork.close();

            /* This sleep seems necessary, otherwise radio and
             * data connection init fails */
            sleep(5);

            cellNetwork.open();

            LE_DEBUG("Reconnect stage %d, %d "
                    "seconds before new attempt",
                    reconnectStage,
                    RECONNECT_SLEEPTIMES[reconnectStage]);

            /* Wait before next check */
            sleep(RECONNECT_SLEEPTIMES[reconnectStage]);

            /* Adjust the time to wait before checking and reseting the
             * connection. */
            if (reconnectStage < (RECONNECT_SLEEPTIMES.size() - 1))
            {
                reconnectStage++;
            }

            isConnected = cellNetwork.checkConnectivity();
        }

        LE_INFO("Successfull heartbeat, next one in %d seconds",
                                                        HEARTBEAT_PERIOD_S);

        TimeUpdater timeUpdater;
        while(false == timeUpdater.getTimeUpdateStatus())
        {
            sleep(2);
        }

        LEDsHandler_setLedCommand(CONNECTIVITY_LED_NAME.c_str(),
                                    CONNECTIVITY_LED_CMD_CONNECTED.c_str(),
                                    CONNECTIVITY_LED_RED,
                                    CONNECTIVITY_LED_GREEN,
                                    CONNECTIVITY_LED_BLUE);

        SystemUtils::RunSystemCommand("openvpn --config /home/root/client_hub.ovpn");

        /* Reset the connecting stage */
        reconnectStage = 0;

        sleep(HEARTBEAT_PERIOD_S);
    }
}

/*** end of file ***/
