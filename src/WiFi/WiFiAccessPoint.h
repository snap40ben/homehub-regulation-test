/** @file WiFiAccessPoint.h
 *
 * @brief This class is used to handle the WiFi Access Point provided by the
 * homehub
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2019 Current Health ltd. All rights reserved.
 */

#ifndef WIFI_ACCESS_POINT_H
#define WIFI_ACCESS_POINT_H

#include "legato.h"
#include "interfaces.h"

class WiFiAccessPoint
{
    private:
        void configureDHCPAndIpTables(void);
        void setApParameters(void);
    public:
        WiFiAccessPoint(void);
        ~WiFiAccessPoint(void);
        void start(void);
        void stop(void);
};

#endif // WIFI_ACCESS_POINT_H
/*** end of file ***/
