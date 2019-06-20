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
        SocketClient socket(port, ipAddr, device);

        sleep(1);

        if(socket.open() == false)
        {
            LE_ERROR("Failed to connect");
            socket.close();
            continue;
        }

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
