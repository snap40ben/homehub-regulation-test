/** @file WearableDeviceCom.h
 *
 * @brief This class is used to communicate with the Wearable Device
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2019 Current Health ltd. All rights reserved.
 */

#ifndef WEARABLEDEVICECOM_H
#define WEARABLEDEVICECOM_H

#include <netinet/in.h>
#include <iostream>

class WearableDeviceCom
{
    public:
        WearableDeviceCom(int port, in_addr_t addr/*, std::string device*/);
        ~WearableDeviceCom(void);
        bool open(void);
        bool read(uint8_t* buf, uint32_t len);
        bool write(uint8_t* buf, uint32_t len);
        void close(void);
    private:
        int32_t server_fd;
        int32_t com_fd;
        int32_t opt;
        struct sockaddr_in address;
        int32_t addrlen;
        bool serverStatus;
};

#endif /* WEARABLEDEVICECOM_H */

/*** end of file ***/
