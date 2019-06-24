/** @file TimeUpdater.h
 *
 * @brief This class provides basic methods to update the system time
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2019 Current Health ltd. All rights reserved.
 */

#ifndef TIME_UPDATER_H
#define TIME_UPDATER_H

class TimeUpdater
{
    public:
        virtual bool getTimeUpdateStatus();
    private:
        static bool isTimeUpdated;
};

#endif /* TIME_UPDATER_H */

/*** end of file ***/
