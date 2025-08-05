#ifndef SCAN_BUFFER_H
#define SCAN_BUFFER_H

#include "./common.h"

// This struct is used for LSM tree scans for Part 3
struct scanBuffer {
    KVPair page[KV_PAIRS_PER_PAGE];
    int numKVPairsInPage = -1;
    int currentPageNumber = -1;
    int maxpages = -1;
};

#endif // SCAN_BUFFER_H