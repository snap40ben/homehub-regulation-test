/** @file SocketClient.h
 *
 ** @brief This class is used to handle a socket client
 **
 * @par
 * COPYRIGHT NOTICE: (c) 2019 Current Health ltd. All rights reserved.
 */

#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H

#include "legato.h"
#include "interfaces.h"
#include <iostream>
#include <netinet/in.h>

class SocketClient
{
    private:
        int32_t socket_fd;
        int32_t opt;
        struct sockaddr_in address;
        int32_t addrlen;
        bool socketStatus;
    public:
        SocketClient(int port,
                    const std::string& ipAddr,
                    const std::string& device);
        ~SocketClient(void);
        bool open(void);
        bool read(uint8_t* buf, uint32_t len);
        bool write(uint8_t* buf, uint32_t len);
        void close(void);
};

#endif // SOCKET_CLIENT_H

/*** end of file ***/
