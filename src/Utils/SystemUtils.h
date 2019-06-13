/** @file SystemUtils.h
 *
 * @brief This class provides basic methods to manipulate the host system
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2019 Current Health ltd. All rights reserved.
 */
#include "legato.h"
#include "interfaces.h"
#include <iostream>

#ifndef SYSTEM_UTILS_H
#define SYSTEM_UTILS_H

class SystemUtils
{
    public:
        static bool RunSystemCommand(std::string commandStringPtr);
};

#endif /* SYSTEM_UTILS_H */

/*** end of file ***/
