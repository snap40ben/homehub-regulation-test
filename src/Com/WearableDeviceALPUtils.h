/** @file WearableDeviceALPUtils.h
 *
 * @brief This file is used to define constants for the
 * Application Layer Protocol with the wearable device
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2019 Current Health ltd. All rights reserved.
 */

#ifndef WEARABLEDEVICEALPUTILS_H
#define WEARABLEDEVICEALPUTILS_H

namespace WearableDeviceALPTypes
{
    enum PayloadType
    {
        Hello, PduPackage
    };
}

namespace WearableDeviceALPConstants
{
    /* ALP Connect */
    const uint8_t ALP_CONNECT[] =
    { 0x10, 0x00, 0x04, 0x41, 0x4C, 0x50, 0x2D, 0xFF, 0xFF, 0xFF, 0xFF, 0x00 };

    /* ALP Connect Ack */
    const uint8_t ALP_CONNECT_ACK[] =
    { 0x20, 0x02, 0x00, 0x00 };

    /* Size of the Publish header */
    const uint8_t PUBLISH_HEADER_SIZE = 12;

    /* PUBLISH NEW: Device is willing to transmit PDUs */
    const uint8_t ALP_PUBLISH_NEW = 0x34;

    /* Alp Publish Ack */
    const uint8_t ALP_PUBACK[] =
    { 0x40, 0x00 };

    /* ALP Disconnect */
    const uint8_t ALP_DISCONNECT[] =
    { 0x14, 0x00 };

    /* HELLO: Device is checking in and requesting a UTC time update */
    const uint8_t ALP_HELLO = 0x3D;

    const uint8_t PUBLISH_MSG_SENSOR_DATA_PUSH = 0xC0;

    /* Size of the payload containing the UTC time requested by the device */
    const uint8_t UTC_PAYLOAD_SIZE = 7;

    /* Size of the timestamp sent after a publish message */
    const uint8_t UTC_TIMESTAMP_SIZE = 4;

    /* Size of the mac address */
    const uint8_t MAC_ADDRESS_SIZE = 6;

    /* Size of PUBLISH Message Type and Length */
    const uint8_t PUBLISH_MSG_TYPE_AND_LEN_SIZE = 5;

    /* Size of CRC */
    const uint8_t CRC_SIZE = 4;

    /* Timeout in seconds before aborting an ALP commmunication */
    const uint8_t ALP_COMMUNICATION_TIMEOUT_SEC = 5;

    /* Number of connection that can be queued before refusing connection
     * from the wearable device */
    const uint8_t DEVICE_COM_SOCKET_CONN_QUEUE_SIZE = 10;

    /* Maximum payload size received from the device.
     * This limit is scaled accordingly to the hardware
     * Used to prevent allocating memory that cannot be afforded */
    const uint32_t MAX_ALP_PAYLOAD_SIZE = 300000;

    /* Port to be used to communicate with the wearable device*/
    const uint16_t ALP_SOCKET_PORT = 8088;
}

#endif /* WEARABLEDEVICEALPUTILS_H */

/*** end of file ***/
