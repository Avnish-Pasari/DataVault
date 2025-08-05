#include "bloom-filters.h"

uint64_t BloomFilter::calculateBitmapSize(int level) {
    // This assumes LSM tree size ratio is 2
    return (1 << level) * MEMTABLE_SIZE * BLOOM_FILTER_BITS_PER_ENTRY;
}

void BloomFilter::hash(uint64_t key, uint64_t hashValue[2]) {;
    MurmurHash3_x64_128(&key, sizeof(key), 0, hashValue);
}

BloomFilter::BloomFilter(int level) : level(level) {
    int bitmapSize = calculateBitmapSize(level);
    this->bitmapSize = bitmapSize;

    bitmap = new char[bitmapSize];
    memset(bitmap, 0, bitmapSize);
}

BloomFilter::~BloomFilter() {
    delete[] bitmap;
}

void BloomFilter::insertKey(uint64_t key) {
    uint64_t hashValue[2];
    hash(key, hashValue);

    for (int i = 0; i < BLOOM_FILTER_NUM_HASH_FUNCTIONS; i++) {
        // use Kirsch-Mitzenmacher-Optimization
        uint64_t hash = (hashValue[0] + i * hashValue[1]) % bitmapSize;
        bitmap[hash] = 1;
    }
}

void BloomFilter::getKeyBitLocationForHashFunction(uint64_t key, int level, 
                            int hashFunctionNumber, int &bfPageNumber, int &bfPageOffset) {
    uint64_t hashValue[2];
    hash(key, hashValue);
    int bitmapSize = calculateBitmapSize(level);
    
    hashFunctionNumber -= 1; // hashFunctionNumber is 1-indexed, convert to 0-indexed
    uint64_t hash = (hashValue[0] + hashFunctionNumber * hashValue[1]) % bitmapSize;

    // Calculate the page number and offset
    // Each page has 4096 bytes
    bfPageNumber = hash / PAGE_SIZE; // 0-indexed
    bfPageOffset = hash % PAGE_SIZE; // 0-indexed
}