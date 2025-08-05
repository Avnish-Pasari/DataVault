#ifndef BUFFER_POOL_TEST_H
#define BUFFER_POOL_TEST_H

#include "../../kv-store/buffer-pool/buffer-pool.h"
#include "../../kv-store/util/namespace-std.h"
#include <iostream>
#include <cassert>

// Helper function to simulate a read I/O from storage
inline KVPair *simulateGetApi() {
    KVPair *kvPairs = new KVPair[KV_PAIRS_PER_PAGE];
    for (int i = 0; i < KV_PAIRS_PER_PAGE; i++) {
        kvPairs[i] = KVPair(i, i);
    }
    return kvPairs;
}

// Helper function to simulate a bloom filter read I/O
inline char *simulateBfGetApi() {
    char *bitmap = new char[PAGE_SIZE];
    for (int i = 0; i < PAGE_SIZE; i++) {
        bitmap[i] = i % 2;
    }
    return bitmap;
}

inline void cleanup(KVPair *kvPairs) {
    delete[] kvPairs;
}

inline void cleanup(char *bitmap) {
    delete[] bitmap;
}

void testBufferPoolInitialization();
void testBufferPoolSingleInsertionAndRetrieval();
void testBufferPoolMultipleInsertions();
void testBufferPoolCollisions();

void testBufferPoolEvictionNoCollisions();
void testBufferPoolEvictionWithCollisions();


void bufferPoolTestAll();

#endif