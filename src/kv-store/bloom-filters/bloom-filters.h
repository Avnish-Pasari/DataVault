#ifndef BLOOM_FILTERS_H
#define BLOOM_FILTERS_H

#include "../util/common.h"
#include "../util/namespace-std.h"
#include "../util/murmurhash/MurmurHash3.h"
#include <cmath>
#include <cstring>

/**
 * @class BloomFilter
 * @brief Implements a Bloom Filter for probabilistic membership testing.
 */
class BloomFilter {
    private: 
        int level;
        char *bitmap;
        uint64_t bitmapSize;

        /**
         * @brief Calculate the size (in bits) of the bitmap for a given level
         * 
         * @param level The level of the LSM tree
         * 
         * @return uint64_t The size of the bitmap in bits
         */
        static uint64_t calculateBitmapSize(int level);

    public:
        /**
         * @brief Construct a new Bloom Filter object with the given level
         * 
         * @param level The level of the LSM tree
         */
        BloomFilter(int level);

        /**
         * @brief Destroy the Bloom Filter object
         */
        ~BloomFilter();

        /**
         * @brief Get the bitmap of the Bloom Filter
         * 
         * @return char* The bitmap
         */
        char *getBitmap() {
            return bitmap;
        }

        /**
         * @brief Get the size of the bitmap in bits
         * 
         * @return uint64_t The size of the bitmap in bits
         */
        uint64_t getBitmapSize() {
            return bitmapSize;
        }

        /**
         * @brief Hash a key to two 64-bit hash values using MurmurHash3
         * 
         * @param key The key to hash
         * @param hashValue The array to store the two hash values
         */
        static void hash(uint64_t key, uint64_t hashValue[2]);


        /**
         * @brief Insert a key into the Bloom Filter
         * 
         * @param key The key to insert
         */
        void insertKey(uint64_t key);

        /**
         * @brief Get the page number and offset of the bit in the bitmap for a given key and hash function
         * 
         * @param key The key to hash and get the bit location for
         * @param level The level of the LSM tree
         * @param hashFunctionNumber The hash function number
         * @param bfPageNumber The page number of the bit in the bitmap (0-indexed)
         * @param bfPageOffset The offset of the bit in the page (0-indexed)
         */
        static void getKeyBitLocationForHashFunction(uint64_t key, int level, 
                            int hashFunctionNumber, int &bfPageNumber, int &bfPageOffset);
};

#endif // BLOOM_FILTERS_H