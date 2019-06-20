/** @file WiFiServerHandler.cpp
 *
 * @brief This component handles the test server for the WiFi interface
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2019 Current Health ltd. All rights reserved.
 */
#include "legato.h"
#include "interfaces.h"
#include "Socket/SocketClient.h"
#include <vector>
#include <arpa/inet.h>
#include <iostream>
#include <vector>

#define TEST_PING_PAYLOAD_START     (0xBE)
#define TEST_PING_PAYLOAD_END       (0xEF)

#define PING_HEADER_SIZE            (5)
#define PING_FOOTER_SIZE            (3)

static const std::string CONNECTIVITY_LED_NAME                  = "RGB_D1";
static const std::string CONNECTIVITY_LED_CMD_CONNECTED         = "On";
static const std::string CONNECTIVITY_LED_CMD_NOT_CONNECTED     = "On";
static const uint8_t CONNECTIVITY_LED_RED_CONNECTED             = 0x00;
static const uint8_t CONNECTIVITY_LED_GREEN_CONNECTED           = 0x30;
static const uint8_t CONNECTIVITY_LED_BLUE_CONNECTED            = 0x00;
static const uint8_t CONNECTIVITY_LED_RED_NOT_CONNECTED         = 0x30;
static const uint8_t CONNECTIVITY_LED_GREEN_NOT_CONNECTED       = 0x30;
static const uint8_t CONNECTIVITY_LED_BLUE_NOT_CONNECTED        = 0x00;

/*!
 * @brief Main function of the WiFiServerHandler component
 * */
COMPONENT_INIT
{
    std::string ipAddr {"18.223.98.61"};
    std::string device {"rmnet_data0"};
    int port {55557};

    while (1)
    {
        LEDsHandler_setLedCommand(CONNECTIVITY_LED_NAME.c_str(),
                                    CONNECTIVITY_LED_CMD_NOT_CONNECTED.c_str(),
                                    CONNECTIVITY_LED_RED_NOT_CONNECTED,
                                    CONNECTIVITY_LED_GREEN_NOT_CONNECTED,
                                    CONNECTIVITY_LED_BLUE_NOT_CONNECTED);

        SocketClient socket(port, ipAddr, device);

        if(socket.open() == false)
        {
            LE_ERROR("Failed to connect");
            socket.close();
            continue;
        }

        LEDsHandler_setLedCommand(CONNECTIVITY_LED_NAME.c_str(),
                                    CONNECTIVITY_LED_CMD_CONNECTED.c_str(),
                                    CONNECTIVITY_LED_RED_CONNECTED,
                                    CONNECTIVITY_LED_GREEN_CONNECTED,
                                    CONNECTIVITY_LED_BLUE_CONNECTED);


        std::vector<uint8_t> header(PING_HEADER_SIZE);

        if(socket.read(&header[0], header.size()) == false)
        {
           LE_ERROR("Failed to read header");
           continue;
        }

        uint16_t length {static_cast<uint16_t>((header[1]) |
                        (static_cast<uint16_t>(header[2] << 8)))};

        LE_INFO("Length of the payload: %d", length);

        std::vector<uint8_t> pingPayload(length);

        if(socket.read(&pingPayload[0], pingPayload.size()) == false)
        {
            LE_ERROR("Failed to read payload");
            continue;
        }

        std::vector<uint8_t> footer(PING_FOOTER_SIZE);

        if(socket.read(&footer[0], footer.size()) == false)
        {
            LE_ERROR("Failed to read footer");
            continue;
        }

        std::vector<uint8_t> pingBack;

        pingBack.insert(pingBack.end(), header.begin(), header.end());
        pingBack.insert(pingBack.end(), pingPayload.begin(), pingPayload.end());
        pingBack.insert(pingBack.end(), footer.begin(), footer.end());

        if(socket.write(&pingBack[0], pingBack.size()) == false)
        {
            LE_ERROR("Failed to send ping back");
            continue;
        }
    }
}

/*** end of file ***/
