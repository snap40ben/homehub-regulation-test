/** @file CellularNetwork.h
 *
 * @brief This class is used to handle cellular connectivity
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2019 Current Health ltd. All rights reserved.
 */

#include "CellularNetwork/CellularNetwork.h"
#include "CellularNetwork/CellularNetworkUtils.h"
#include <curl/curl.h>
#include "HttpStatusCode/HttpStatusCode_C++.h"

using namespace CellularNetworkConstants;

/*!
 * @brief Constructor for CellularNetwork
 *
 * */
CellularNetwork::CellularNetwork()
{

}

/*!
 * @brief Destructor for CellularNetwork
 *
 * */
CellularNetwork::~CellularNetwork()
{

}

/*!
 * @brief Start the cellular connectivity
 *
 * @return None
 */
void CellularNetwork::open(void)
{
    le_onoff_t power = LE_OFF;
    le_mrc_GetRadioPower(&power);
    if (power == LE_OFF)
    {
        LE_INFO("Turn radio ON");
        le_mrc_SetRadioPower(LE_ON);
    }

    le_mdc_ConState_t state = LE_MDC_DISCONNECTED;
    /* Check the state */
    LE_ASSERT(LE_OK ==
            le_mdc_GetSessionState(le_mdc_GetProfile(TWILIO_PROFILE_INDEX),
                                    &state));

    /* If already in use, disconnect the session */
    if (LE_MDC_DISCONNECTED != state)
    {
        LE_INFO("Already in use, disconnect");
        LE_ASSERT(LE_OK ==
                le_mdc_StopSession(le_mdc_GetProfile(TWILIO_PROFILE_INDEX)));
    }

    /* Sleeps between configuration steps seems necessary, otherwise the modem
     * drivers might either not work at all or misbehave
     */
    sleep(2);

    le_mdc_SetPDP(le_mdc_GetProfile(TWILIO_PROFILE_INDEX), TWILIO_PDP);

    /* Refer to previous comment */
    sleep(2);

    le_mdc_SetAPN(le_mdc_GetProfile(TWILIO_PROFILE_INDEX), TWILIO_APN.c_str());

    /* Refer to previous comment */
    sleep(2);

    LE_INFO("Connect");

    le_result_t result =
            le_mdc_StartSession(le_mdc_GetProfile(TWILIO_PROFILE_INDEX));

    /* Refer to previous comment */
    sleep(2);

    if (result < 0)
    {
        LE_ERROR("Couldn't start session: %d", result);
        le_mdc_StopSession(le_mdc_GetProfile(TWILIO_PROFILE_INDEX));
    }
    else
    {
        LE_ASSERT_OK(le_mdc_ResetBytesCounter());
        setNetworkConfiguration(le_mdc_GetProfile(TWILIO_PROFILE_INDEX));
        setAMSConfig();
    }
}

/*!
 * @brief Stop the WiFi access point
 *
 * @return None
 */
void CellularNetwork::close(void)
{
    le_mdc_ConState_t state = LE_MDC_DISCONNECTED;

    /* Check the state of the session */
    LE_ASSERT(LE_OK ==
            le_mdc_GetSessionState(le_mdc_GetProfile(TWILIO_PROFILE_INDEX),
                                    &state));

    /* If already connected, disconnect the session */
    if (LE_MDC_CONNECTED == state)
    {
        LE_INFO("Disconnect");
        LE_ASSERT(LE_OK ==
                le_mdc_StopSession(le_mdc_GetProfile(TWILIO_PROFILE_INDEX)));
    }

    /* Check if the radio is powered. Turn it OFF if yes */
    le_onoff_t power = LE_ON;

    le_mrc_GetRadioPower(&power);

    if (power == LE_ON)
    {
        LE_INFO("Turn radio OFF");
        le_mrc_SetRadioPower(LE_OFF);
    }
}

/*!
 * @brief Configure the network according to the given profile
 *
 * @param[in] profileRef      Modem data connection profile
 *
 * @return Void
 * */
void CellularNetwork::setNetworkConfiguration(le_mdc_ProfileRef_t profileRef)
{
    char ipAddr[100] = {0};
    char gatewayAddr[100] = {0};
    char dns1Addr[100] = {0};
    char dns2Addr[100] = {0};
    char systemCmd[200] = {0};
    FILE* resolvFilePtr;
    le_mdc_ConState_t state = LE_MDC_DISCONNECTED;
    mode_t oldMask;

    // Check the state
    LE_ASSERT( le_mdc_GetSessionState(profileRef, &state) == LE_OK );
    LE_ASSERT( state == LE_MDC_CONNECTED );

    // Get IP, gateway and DNS addresses for IPv4 or IPv6 connectivity
    if ( le_mdc_IsIPv4(profileRef) )
    {
        LE_ASSERT(le_mdc_GetIPv4Address(profileRef, ipAddr, sizeof(ipAddr))
                                                                    == LE_OK);
        LE_INFO("%s", ipAddr);

        LE_ASSERT(le_mdc_GetIPv4GatewayAddress(profileRef, gatewayAddr,
                                                sizeof(gatewayAddr)) == LE_OK);
        LE_INFO("%s", gatewayAddr);

        LE_ASSERT(le_mdc_GetIPv4DNSAddresses( profileRef,
                                              dns1Addr, sizeof(dns1Addr),
                                              dns2Addr, sizeof(dns2Addr))
                                              == LE_OK );
        LE_INFO("%s", dns1Addr);
        LE_INFO("%s", dns2Addr);

        sleep(5);

        snprintf(systemCmd, sizeof(systemCmd),
                                "/sbin/route add default gw %s", gatewayAddr);
    }
    else if ( le_mdc_IsIPv6(profileRef) )
    {
        LE_ASSERT(le_mdc_GetIPv6Address(profileRef, ipAddr,
                                                sizeof(ipAddr)) == LE_OK);
        LE_INFO("%s", ipAddr);

        LE_ASSERT(le_mdc_GetIPv6GatewayAddress(profileRef, gatewayAddr,
                                                sizeof(gatewayAddr)) == LE_OK);
        LE_INFO("%s", gatewayAddr);

        LE_ASSERT(le_mdc_GetIPv6DNSAddresses( profileRef,
                                              dns1Addr, sizeof(dns1Addr),
                                              dns2Addr, sizeof(dns2Addr))
                                                        == LE_OK );
        LE_INFO("%s", dns1Addr);
        LE_INFO("%s", dns2Addr);

        snprintf(systemCmd, sizeof(systemCmd),
                        "/sbin/route -A inet6 add default gw %s", gatewayAddr);
    }

    sleep(5);

    LE_DEBUG("%s", systemCmd);
    LE_ASSERT( system(systemCmd) == 0 );

    // allow fopen to create file with mode=644
    oldMask = umask(022);

    // open the resolver configuration
    resolvFilePtr = fopen("/etc/resolv.conf", "w+");

    if (resolvFilePtr == NULL)
    {
        LE_ERROR("Unable to open resolv.conf: %m");
    }
    LE_ASSERT( resolvFilePtr != NULL );

    LE_ASSERT( fprintf(resolvFilePtr, "nameserver %s\n", dns1Addr) > 0 );

    if (dns2Addr[0] != '\0')
    {
        LE_ASSERT( fprintf(resolvFilePtr, "nameserver %s\n", dns2Addr) > 0 );
    }

    LE_ASSERT( fclose(resolvFilePtr) == 0 );

    // restore old mask
    umask(oldMask);
}

/*!
 * @brief Check if the module is connected to the internet by  pinging
 * google's DNS
 *
 * @return True if connected, false if not
 * */
bool CellularNetwork::checkConnectivity()
{
    bool status = true;

    char systemCmd[200] = {0};

    /* Something's wrong, but it might be Current Health server only */
    /* Double check with google's DNS to confirm */
    if (le_mdc_IsIPv4(le_mdc_GetProfile(TWILIO_PROFILE_INDEX)))
    {
        snprintf(systemCmd, sizeof(systemCmd),
                "ping -c 4 8.8.8.8");
    }
    else if (le_mdc_IsIPv6(le_mdc_GetProfile(TWILIO_PROFILE_INDEX)))
    {
        snprintf(systemCmd, sizeof(systemCmd),
                "ping6 -c 4 8.8.8.8");
    }
    else
    {
        LE_WARN("Couldn't get Session type. Consider it's IPV4");
        snprintf(systemCmd, sizeof(systemCmd),
                            "ping -c 4 8.8.8.8");
    }

    if(system(systemCmd))
    {
        LE_ERROR("Google DNS ping failure");
        status = false;
    }
    else
    {
        LE_INFO("Google DNS ping success");
    }

    return status;
}

/*!
 * @brief Configure the AirVantage Management Services settings using AT
 * commands
 *
 * @return Void
 * */
void CellularNetwork::setAMSConfig()
{
    LE_INFO("AMS configuration");

    /* Open the serial port used to send the AT commands */
    int fd = ::open("/dev/ttyAT", O_RDWR | O_NOCTTY | O_NONBLOCK);

    le_atClient_DeviceRef_t DevRef = le_atClient_Start(fd);

    for (uint8_t cnt = 0 ; cnt < AT_STARTUP_CMDS.size(); cnt++)
    {
        le_atClient_CmdRef_t cmdRef = le_atClient_Create();

        LE_ASSERT(le_atClient_SetDevice(cmdRef, DevRef) == LE_OK);

        LE_ASSERT(le_atClient_SetCommand(cmdRef,
                                AT_STARTUP_CMDS[cnt].c_str()) == LE_OK);

        LE_ASSERT(le_atClient_SetFinalResponse(cmdRef, "OK") == LE_OK);

        LE_ASSERT(le_atClient_Send(cmdRef) == LE_OK);

        LE_ASSERT(le_atClient_Delete(cmdRef) == LE_OK);
    }

    le_atClient_Stop(DevRef);

    ::close(fd);
}



/*** end of file ***/
