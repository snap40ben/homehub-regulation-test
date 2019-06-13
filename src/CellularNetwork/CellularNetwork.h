/** @file CellularNetwork.h
 *
 * @brief This class is used to handle cellular connectivity
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2019 Current Health ltd. All rights reserved.
 */

#ifndef CELLULAR_NETWORK_H
#define CELLULAR_NETWORK_H

#include "legato.h"
#include "interfaces.h"

class CellularNetwork
{
    private:
        void setNetworkConfiguration(le_mdc_ProfileRef_t profileRef);
        void setAMSConfig();

    public:
        CellularNetwork(void);
        ~CellularNetwork(void);
        void open(void);
        void close(void);
        bool checkConnectivity(void);
};

#endif // CELLULAR_NETWORK_H
/*** end of file ***/
