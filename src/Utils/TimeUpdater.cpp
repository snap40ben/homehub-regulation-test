/** @file TimeUpdater.cpp
 *
 * @brief This class provides basic methods to update the system time
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2019 Current Health ltd. All rights reserved.
 */

#include "legato.h"
#include "interfaces.h"
#include "Utils/TimeUpdater.h"
#include "Utils/TimeUpdaterUtils.h"
#include "Utils/SystemUtils.h"

using namespace TimeUpdaterConstants;

/* Initialize the static member of TimeUpdater that allows to tell if the
 * current system time is accurate */
bool TimeUpdater::isTimeUpdated = false;

/*!
 * @brief Update the time. Return true if the time was updated at least once.
 *
 * @return True if the system time is updated since app start, false otherwise.
 */
bool TimeUpdater::getTimeUpdateStatus()
{
    if (!TimeUpdater::isTimeUpdated)
    {
        if (SystemUtils::RunSystemCommand(TIME_UPDATE_CMD.c_str()))
        {
            LE_INFO("Time updated successfully!");
            TimeUpdater::isTimeUpdated = true;
        }
    }

    return TimeUpdater::isTimeUpdated;
}

/*** end of file ***/
