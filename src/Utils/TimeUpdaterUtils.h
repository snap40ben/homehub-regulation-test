/** @file TimeUpdaterUtils.h
 *
 * @brief This file provides constants definition used to update the system time
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2019 Current Health ltd. All rights reserved.
 */

#ifndef TIME_UPDATER_UTILS_H
#define TIME_UPDATER_UTILS_H

#include <iostream>

namespace TimeUpdaterConstants
{
    const std::string NTP_SERVER_HOSTNAME = "pool.ntp.org";
    const std::string NTPD_CMD = "/usr/sbin/ntpd -d -q -n -p ";
    const std::string TIME_UPDATE_CMD = NTPD_CMD + " " + NTP_SERVER_HOSTNAME;
}

#endif /* TIME_UPDATER_UTILS_H */

/*** end of file ***/
