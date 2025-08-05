#ifndef _BINARY_SEARCH_H_
#define _BINARY_SEARCH_H_

#include <stdio.h>
#include <iostream>
#include "./namespace-std.h"
#include "../util/common.h"

// Performs binary search on the input KV pairs and returns the index to the key.
// If the key does not exist, the function returns -1
inline int KV_BinarySearch(KVPair *kvPairs, int size, uint64_t key) {
    int left = 0;
    int right = size - 1;
    while (left <= right) { 
        int mid = (left + right) / 2;
        if (kvPairs[mid].getKey() == key) {
            return mid;
        } 
        else if (kvPairs[mid].getKey() < key) {
            left = mid + 1;
        } 
        else {
            right = mid - 1;
        }
    }

    return -1;
}

#endif /* _BINARY_SEARCH_H_ */