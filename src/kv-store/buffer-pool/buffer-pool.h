#ifndef BUFFERPOOL_H
#define BUFFERPOOL_H

#include "../util/common.h"
#include "../util/namespace-std.h"
#include "../util/murmurhash/MurmurHash3.h"
#include "../util/deque/deque.h"
#include <iostream>
#include <cstring>

/**
 * @class Frame
 * @brief Represents a frame in the buffer pool
 */
class Frame {
    private:
        KVPair page[KV_PAIRS_PER_PAGE];
        int numKVPairs;
        char bitmap[PAGE_SIZE];
        int numBitsInBloomFilter;
        string pageId;
        Frame *next;
    public:
        /**
         * @brief Construct a new Frame object
         */
        Frame();

        /**
         * @brief Construct a new Frame object that stores KV Pairs 
         * 
         * @param page The KV pairs to store in the frame
         * @param numKVPairs The number of KV pairs
         * @param pageId The page ID
         */
        Frame(KVPair *page, int numKVPairs, string pageId);

        /**
         * @brief Construct a new Frame object that stores a Bloom filter page
         * 
         * @param bitmap The bitmap of the Bloom filter for the given page
         * @param numBitsInBloomFilter The number of bits in the Bloom filter page
         * @param pageId The page ID
         */
        Frame(char *bitmap, int numBitsInBloomFilter, string pageId);

        /**
         * @brief Return the KV pairs stored in the frame
         * 
         * @return KVPair* The KV pairs
         * @throws runtime_error if the frame contains Bloom filter data
         */
        KVPair *getPage() { 
            if (pageId.substr(0, 2) == "BF") {
                throw std::runtime_error("The page contains Bloom filter data, not KV pairs.");
            }
            return page; 
        }
        
        /**
         * @brief Return the number of KV pairs stored in the frame
         * 
         * @return int The number of KV pairs
         * @throws runtime_error if the frame contains Bloom filter data
         */
        int getNumKVPairs() { 
            if (pageId.substr(0, 2) == "BF") {
                throw std::runtime_error("The page contains Bloom filter data, not KV pairs.");
            }
            return numKVPairs; 
        }

        /**
         * @brief Return the bitmap stored in the frame
         * 
         * @return char* The bitmap
         * @throws runtime_error if the frame contains KV pairs
         */
        char *getBitmap() { 
            if (pageId.substr(0, 2) != "BF") {
                throw std::runtime_error("The page contains KV pairs, not Bloom filter data.");
            }
            return bitmap; 
        }

        /**
         * @brief Return the number of bits in the Bloom filter stored in the frame
         * 
         * @return int The number of bits in the Bloom filter
         * @throws runtime_error if the frame contains KV pairs
         */
        int getNumBitsInBloomFilter() { 
            if (pageId.substr(0, 2) != "BF") {
                throw std::runtime_error("The page contains KV pairs, not Bloom filter data.");
            }
            return numBitsInBloomFilter; 
        }

        /**
         * @brief Return the page ID of the frame
         * 
         * @return string The page ID
         */
        string getPageId() { return pageId; }

        /**
         * @brief Return the next frame in the chain
         * 
         * @return Frame* The next frame
         */
        Frame *getNext() { return next; }

        /**
         * @brief Set the next frame in the chain
         * 
         * @param newNext The new next frame
         */
        void setNext(Frame *newNext) { next = newNext; }

        /**
         * @brief Chain the given frame to the end of the current frame
         * 
         * @param newFrame The frame to chain
         */
        void chain(Frame *newFrame);
};

/**
 * @class BufferPool
 * @brief Implements a Buffer Pool for storing frequently accessed pages
 */
class BufferPool {
    private:
        Frame *hashTable[BUFFER_POOL_SIZE];
        int size;
        DoubleEndedQueue lruQueue;

        /**
         * @brief Hash function to determine the index in the hash table
         * 
         * @param pageId The page ID
         * 
         * @return int The index in the hash table
         */
        int hash(string pageId);

        /**
         * @brief Evict the least recently used page from the buffer pool
         * 
         * The least recently used page is the one at the front of the LRU queue
         */
        void evictPage();

        /**
         * @brief Update the LRU queue with the given pageId
         * 
         * @param pageId The page ID
         */
        void updateLRU(string pageId);
    public:
        /**
         * @brief Construct a new Buffer Pool object
         */
        BufferPool();

        /**
         * @brief Destroy the Buffer Pool object
         */
        ~BufferPool();

        /**
         * @brief Get the hash table of the buffer pool
         * 
         * @return Frame** The hash table
         */
        Frame **getHashTable() { return hashTable; }

        /**
         * @brief Get the size of the buffer pool
         * 
         * @return int The size of the buffer pool
         */
        int getSize() { return size; }

        /**
         * @brief Get the LRU queue of the buffer pool
         * 
         * @return DoubleEndedQueue& The LRU queue
         */
        DoubleEndedQueue& getLRUQueue() { return lruQueue; }

        /**
         * @brief Put the page contianing KV pairs into the buffer pool
         * 
         * If the page is already in the buffer pool, update the LRU queue
         * If the buffer pool is at max capacity, evict the LRU page
         * 
         * @param pageId The page ID
         * @param page The page containing KV pairs
         * @param numKVPairs The number of KV pairs in the page
         * 
         * @return int 1 if the page is successfully added to the buffer pool
         */
        int put(string pageId, KVPair *page, int numKVPairs);

        /**
         * @brief Get the page storing KVPairs with the given pageId from the buffer pool
         * 
         * If the page is found:
         *  1) Update the LRU queue
         *  2) Copy the page into the outputPage
         *  3) Copy the number of KV pairs into numKVPairs
         * 
         * @param pageId The page ID
         * @param outputPage The output page to copy into and store the KV Pairs
         * @param numKVPairs The number of KV Pairs in the page is copied into this variable
         * 
         * @return int 1 if the page is found in the buffer pool
         *             0 if the page is not found in the buffer pool
         */
        int get(string pageId, KVPair *outputPage, int *numKVPairs);

        /**
         * @brief Put the page containing a Bloom filter into the buffer pool
         * 
         * If the page is already in the buffer pool, update the LRU queue
         * If the buffer pool is at max capacity, evict the LRU page
         * 
         * @param pageId The page ID
         * @param bitmap The bitmap of the Bloom filter
         * @param numBitsInBloomFilter The number of bits in the Bloom filter
         * 
         * @return int 1 if the page is successfully added to the buffer pool
         */
        int putBfPage(string pageId, char *bitmap, int numBitsInBloomFilter);

        /**
         * @brief Get the page storing a Bloom filter with the given pageId from the buffer pool
         * 
         * If the page is found:
         *  1) Update the LRU queue
         *  2) Copy the bitmap into the outputBitmap
         *  3) Copy the number of bits in the Bloom filter into numBitsInBloomFilter
         * 
         * @param pageId The page ID
         * @param outputBitmap The output bitmap to copy into and store the Bloom filter
         * @param numBitsInBloomFilter The number of bits in the Bloom filter is copied into this variable
         * 
         * @return int 1 if the page is found in the buffer pool
         *             0 if the page is not found in the buffer pool
         */
        int getBfPage(string pageId, char *outputBitmap, int *numBitsInBloomFilter);
};

#endif // BUFFERPOOL_H