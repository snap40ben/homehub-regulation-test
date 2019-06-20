/** @file SocketClient.cpp
 *
 * @brief This class is used to handle a socket client
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2019 Current Health ltd. All rights reserved.
 */

#include "legato.h"
#include "interfaces.h"
#include "Utils/SystemUtils.h"
#include "Socket/SocketClient.h"
#include <signal.h>
#include <arpa/inet.h>

/*!
 * @brief Constructor for SocketClient. This initialize the socket client
 * and launch it.
 * */
SocketClient::SocketClient (int port,
                            const std::string& ipAddr,
                            const std::string& device) :
                            addrlen(sizeof(address)), socketStatus(true)
{
    LE_INFO("Create socket");

    /* Create the TCP socket file descriptor using IPv4 */
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd < 0)
    {
        LE_ERROR("Couldn't create the socket");
    }
    else
    {
        LE_INFO("Socket created successfully");
    }

    if (socket_fd == 0)
    {
        LE_ERROR("Socket file descriptor is NULL");
        socketStatus = false;
    }

    if (socketStatus)
    {
        if (setsockopt(socket_fd, SOL_SOCKET, SO_BINDTODEVICE,
                            device.c_str(), device.length()))
        {
            LE_ERROR("setsockopt failure on SO_BINDTODEVICE");
            socketStatus = false;
        }
    }

    if (socketStatus)
    {
        address.sin_family = AF_INET;
        inet_pton(AF_INET, ipAddr.c_str(), &address.sin_addr);
        address.sin_port = htons(port);
    }
}

/*!
 * @brief Destructor for SocketClient.
 * Close the TCP sockets.
 * */
SocketClient::~SocketClient(void)
{
    LE_INFO("Server socket closed");
    SocketClient::close();
    ::close(socket_fd);
}

/*!
 * @brief Connect to the socket server
 *
 * @return Status of the operation.
 */
bool SocketClient::open(void)
{
    bool status = true;

    if (!socketStatus)
    {
        LE_ERROR("Socket is not initialized successfully");
        status = false;
    }

    if (status)
    {
        if (connect(socket_fd, (const sockaddr*)&address, sizeof(address)) < 0)
        {
            LE_ERROR("Failed to connect to the socket: %s", strerror(errno));
            status = false;
        }
        else
        {
            LE_INFO("Socket connected");
        }
    }

    return status;
}

/*!
 * @brief Send data to the socket
 *
 * @param[in] buf   Pointer to the buffer of bytes to be sent
 * @param[in] len   Number of bytes to send
 *
 * @return Status of the operation.
 */
bool SocketClient::write(uint8_t* buf, uint32_t len)
{
    bool status = true;
    int32_t bytesSentNb = 0;

    if (!socketStatus)
    {
        LE_ERROR("Server is not initialized successfully");
        status = false;
    }

    if (buf == NULL)
    {
        LE_ERROR("Provided buffer is NULL");
        status = false;
    }

    if (status)
    {
        if (socket_fd < 0)
        {
            LE_ERROR("socket_fd descriptor is not valid. "
                    "Please make sure start method was successful.");
            status = false;
        }
    }

    if (status)
    {
        /* Send the buffer through the TCP socket */
        bytesSentNb = send(socket_fd, buf, len, 0);

        if (bytesSentNb < 0)
        {
            LE_ERROR("Error while transmitting to the socket");
            status = false;
        }
        else if ((uint32_t) bytesSentNb != len)
        {
            LE_ERROR("Error, only %d bytes transmitted out of %d", bytesSentNb,
                    len);
            status = false;
        }
    }

    if (status)
    {
        LE_INFO("%d bytes sent successfully", bytesSentNb);
    }

    return status;
}

/*!
 * @brief Receive data from the socket
 *
 * @param[out] buf  Pointer to the buffer to put the bytes read into
 * @param[in] len   Number of bytes to read
 *
 * @return Status of the operation.
 */
bool SocketClient::read(uint8_t* buf, uint32_t len)
{
    bool status = true;
    uint32_t bytesReceivedNb = 0;

    if (!socketStatus)
    {
        LE_ERROR("Server is not initialized successfully");
        status = false;
    }

    if (buf == NULL)
    {
        LE_ERROR("Provided buffer is NULL");
        status = false;
    }

    if (status)
    {
        if (socket_fd < 0)
        {
            LE_ERROR("socket_fd descriptor is not valid. "
                    "Please make sure start method was successful.");
            status = false;
        }
    }

    /* Reading from the TCP socket might need to be in several parts
     * depending on the size of the payload expected */
    while (bytesReceivedNb != len)
    {
        /* Read from the TCP socket */
        /* Use "::" to explicitly refer to the global namespace
         * socket recv() function from <sys/socket.h>*/
        int32_t comStatus = ::recv(socket_fd, buf + bytesReceivedNb,
                                    len - bytesReceivedNb, 0);

        if (comStatus < 0)
        {
            LE_ERROR("Error reading from the socket");
            status = false;
            break;
        }
        else if (comStatus == 0)
        {
            LE_ERROR("Socket was closed by the server");
            status = false;
            break;
        }
        else
        {
            /* We successfully received another chunk of bytes */
            bytesReceivedNb += comStatus;
        }
    }

    if (status)
    {
        LE_INFO("%u bytes received successfully", bytesReceivedNb);
    }
    else
    {
        LE_ERROR("%u bytes received, expected %u", bytesReceivedNb, len);
    }
    return status;
}

/*!
 * @brief Close the TCP socket
 */
void SocketClient::close(void)
{
    LE_INFO("Socket closed");

    /* Use "::" to explicitly refer to the global namespace
     * close() function from <unistd.h>*/
    ::shutdown(socket_fd, SHUT_RDWR);
}

/*** end of file ***/


/*** end of file ***/
