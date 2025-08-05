#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <iostream>
#include <cstdint>
#include <vector>
#include <cmath>
#include "./namespace-std.h"

#define NAME_LENGTH 256 // Maximum length of a file name
#define PATH_LENGTH 4096 // Maximum length of a file path

#define PAGES_PER_SST 256 // pages per memtable, 1MB -> 256 x 4KB pages
#define KV_PAIRS_PER_PAGE 256 // 256 x 16B pages = 4KB. This should never change. 
#define KV_PAIR_SIZE 16     // 8 Bytes for keys and 8 Bytes for value. This should never change. 
#define PAGE_SIZE (KV_PAIRS_PER_PAGE * KV_PAIR_SIZE)    // 4096 bytes =  4KB

#define MEMTABLE_SIZE (PAGES_PER_SST * KV_PAIRS_PER_PAGE) // number of KV-pairs in memtable, 1MB memtable -> 65536x 16B pages

#define BUFFER_POOL_SIZE 2560 // 10MB -> 2560 x 4KB pages
#define B_TREE_INTERNAL_NODE_SIZE (KV_PAIRS_PER_PAGE - 1) // 255
#define B_TREE_FANOUT (KV_PAIRS_PER_PAGE) // 256

#define LSM_TREE_SIZE_RATIO 2

constexpr int computeNumHashFunctions(int bitsPerEntry) {
    return static_cast<int>(ceil(log(2.0) * static_cast<double>(bitsPerEntry)));
}
const int BLOOM_FILTER_BITS_PER_ENTRY = 8;
const int BLOOM_FILTER_NUM_HASH_FUNCTIONS = computeNumHashFunctions(BLOOM_FILTER_BITS_PER_ENTRY);

class KVPair {
    private:
        uint64_t key;
        int64_t value;
    public:
        KVPair() {}
        KVPair(uint64_t key, int64_t value) : key(key), value(value) {}
        uint64_t getKey() { return key; }
        int64_t getValue() { return value; }

        bool operator<(const KVPair& other) const {
            return key < other.key;
        }
};

#endif /* _COMMON_H_ */