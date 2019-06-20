/** @file WearableDeviceCom.cpp
 *
 * @brief This class is used to communicate with the Wearable Device
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2019 Current Health ltd. All rights reserved.
 */

#include "legato.h"
#include "interfaces.h"
#include "Com/WearableDeviceCom.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Com/WearableDeviceALPUtils.h"

using namespace WearableDeviceALPConstants;

/*!
 * @brief Constructor for WearableDeviceCom. This initialize the socket server
 * and launch it. A call to open() will then allow to receive the next
 * device connection.
 * */
WearableDeviceCom::WearableDeviceCom (int port, in_addr_t addr, std::string device) : opt(1), addrlen(sizeof(address)), serverStatus(true)
{
    struct timeval alpTimeout;
    alpTimeout.tv_sec = ALP_COMMUNICATION_TIMEOUT_SEC;
    alpTimeout.tv_usec = 0;

    LE_INFO("Create socket");

    /* Create the TCP socket file descriptor using IPv4 */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0)
    {
        LE_ERROR("Couldn't create the socket");
    }
    else
    {
        LE_INFO("Socket created successfully");
    }

    if (server_fd == 0)
    {
        LE_ERROR("Socket file descriptor is NULL");
        serverStatus = false;
    }

    if (serverStatus)
    {
        if (setsockopt(server_fd, SOL_SOCKET,
        SO_REUSEADDR | SO_REUSEPORT | SO_KEEPALIVE, &opt, sizeof(opt)) < 0)
        {
            LE_ERROR("setsockopt failure");
            serverStatus = false;
        }
        else if (setsockopt(server_fd, SOL_SOCKET,
                SO_RCVTIMEO, (const char*)&alpTimeout, sizeof(alpTimeout)))
        {
            LE_ERROR("setsockopt failure");
            serverStatus = false;
        }
        else if (setsockopt(server_fd, SOL_SOCKET, SO_BINDTODEVICE,
                            device.c_str(), device.length()))
        {
            LE_ERROR("setsockopt failure on SO_BINDTODEVICE");
            serverStatus = false;
        }
    }

    if (serverStatus)
    {
        address.sin_family = AF_INET;
        address.sin_port = htons(port);

        /* Attach the socket to the defined port */
        if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0)
        {
            LE_ERROR("Failed to bind the socket to port %d", port);
            serverStatus = false;
        }
    }

    if (serverStatus)
    {
        /* Start the server */
        if (listen(server_fd, DEVICE_COM_SOCKET_CONN_QUEUE_SIZE) < 0)
        {
            LE_ERROR("Failed to start listening: %s", strerror(errno));
            serverStatus = false;
        }
        else
        {
            LE_INFO("Starting server on port %d", port);
        }
    }
}

/*!
 * @brief Destructor for WearableDeviceCom.
 * Close the TCP sockets.
 * */
WearableDeviceCom::~WearableDeviceCom(void)
{
    LE_INFO("Server socket closed");
    WearableDeviceCom::close();
    ::close(server_fd);
}

/*!
 * @brief Wait and accept the next device connection to the server
 *
 * @return Status of the operation.
 */
bool WearableDeviceCom::open(void)
{
    bool status = true;

    if (!serverStatus)
    {
        LE_ERROR("Server is not initialized successfully");
        status = false;
    }

    if (status)
    {
        LE_INFO("Waiting for a device to connect");

        do
        {
            /* Wait for a device to connect */
            com_fd = accept(server_fd, (struct sockaddr *) &address,
                    (socklen_t*) &addrlen);
        }
        while((com_fd < 0) && (errno == EAGAIN));

        if (com_fd < 0)
        {
            LE_ERROR("Failed to accept a new device: %s", strerror(errno));
            status = false;
        }
        else
        {
            LE_INFO("New device connected");
        }
    }

    return status;
}

/*!
 * @brief Send data to the wearable device
 *
 * @param[in] buf	Pointer to the buffer of bytes to be sent
 * @param[in] len	Number of bytes to send
 *
 * @return Status of the operation.
 */
bool WearableDeviceCom::write(uint8_t* buf, uint32_t len)
{
    bool status = true;
    int32_t bytesSentNb = 0;

    if (!serverStatus)
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
        if (com_fd < 0)
        {
            LE_ERROR("com_fd descriptor is not valid. "
                    "Please make sure start method was successful.");
            status = false;
        }
    }

    if (status)
    {
        /* Send the buffer through the TCP socket */
        bytesSentNb = send(com_fd, buf, len, 0);

        if (bytesSentNb < 0)
        {
            LE_ERROR("Error while transmitting to the device");
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
 * @brief Receive data from the wearable device
 *
 * @param[out] buf	Pointer to the buffer to put the bytes read into
 * @param[in] len	Number of bytes to read
 *
 * @return Status of the operation.
 */
bool WearableDeviceCom::read(uint8_t* buf, uint32_t len)
{
    bool status = true;
    uint32_t bytesReceivedNb = 0;

    if (!serverStatus)
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
        if (com_fd < 0)
        {
            LE_ERROR("com_fd descriptor is not valid. "
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
        int32_t comStatus = ::recv(com_fd, buf + bytesReceivedNb,
                                    len - bytesReceivedNb, 0);

        if (comStatus < 0)
        {
            LE_ERROR("Error reading from the socket");
            status = false;
            break;
        }
        else if (comStatus == 0)
        {
            LE_ERROR("Socket was closed by the client");
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
 * @brief Close the TCP socket with the wearable device
 */
void WearableDeviceCom::close(void)
{
    LE_INFO("Socket closed");

    /* Use "::" to explicitly refer to the global namespace
     * close() function from <unistd.h>*/
    ::close(com_fd);
}

/*** end of file ***/
