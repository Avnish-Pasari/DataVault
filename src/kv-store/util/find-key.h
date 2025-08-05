#ifndef _FIND_KEY_H_
#define _FIND_KEY_H_

#include <stdio.h>
#include <iostream>
#include "./namespace-std.h"
#include "../util/common.h"

// If the key exists in the KV-Pairs vector, return true. Otherwise, return false.
inline bool findKeyInVector(vector<KVPair> &kvPairs, const uint64_t &key) {
    for (int i = 0; i < kvPairs.size(); i++) {
        if (kvPairs[i].getKey() == key) {
            return true;
        }
    }
    return false;
}

#endif /* _FIND_KEY_H_ */