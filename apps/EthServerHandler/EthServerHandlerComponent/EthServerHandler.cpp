/** @file WiFiServerHandler.cpp
 *
 * @brief This component handles the test server for the WiFi interface
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2019 Current Health ltd. All rights reserved.
 */
#include "legato.h"
#include "interfaces.h"
#include "Com/WearableDeviceCom.h"
#include <vector>
#include <arpa/inet.h>


#define TEST_PING_PAYLOAD_START     (0xBE)
#define TEST_PING_PAYLOAD_END       (0xEF)

#define PING_HEADER_SIZE            (5)
#define PING_FOOTER_SIZE            (3)

/*!
 * @brief Main function of the WiFiServerHandler component
 * */
COMPONENT_INIT
{
    while (1)
    {
        WearableDeviceCom server(55555, INADDR_ANY, "eth0");

        while(1)
        {
            if (!server.open())
            {
                break;
            }

            std::vector<uint8_t> header(PING_HEADER_SIZE);
            server.read(&header[0], header.size());

            uint16_t length = header[1] | (header[2] << 8);

            LE_INFO("Received a length: %d", length);

            std::vector<uint8_t> payload(length);

            server.read(&payload[0], payload.size());

            std::vector<uint8_t> footer(PING_FOOTER_SIZE);
            server.read(&footer[0], footer.size());

            std::vector<uint8_t> pingBack;

            pingBack.insert(pingBack.end(), header.begin(), header.end());
            pingBack.insert(pingBack.end(), payload.begin(), payload.end());
            pingBack.insert(pingBack.end(), footer.begin(), footer.end());

            LE_INFO("Pingback size: %d", pingBack.size());

            server.write((uint8_t*)(&pingBack[0]), pingBack.size());
        }

        sleep(5);
    }
}

/*** end of file ***/
