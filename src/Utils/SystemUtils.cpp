/** @file SystemUtils.cpp
 *
 * @brief This class provides basic methods to manipulate the host system
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2019 Current Health ltd. All rights reserved.
 */

#include "Utils/SystemUtils.h"

using namespace std;

/*!
 * @brief Runs a system command on linux host.
 *
 * @param[in] commandStringPtr   String command to execute
 *
 * @return Status of the operation.
 */
bool SystemUtils::RunSystemCommand(string commandStringPtr)
{
    int systemResult;
    bool status = true;

    if (status)
    {
        systemResult = system(commandStringPtr.c_str());

        /* Return value of -1 means that the fork()
         * has failed (see man system). */
        if (0 == WEXITSTATUS(systemResult))
        {
            LE_INFO("Success: %s", commandStringPtr.c_str());
        }
        else
        {
            LE_ERROR("Error %s Failed: (%d)",
                                commandStringPtr.c_str(),
                                systemResult);
            status = false;
        }
    }

    return status;
}

/*** end of file ***/
