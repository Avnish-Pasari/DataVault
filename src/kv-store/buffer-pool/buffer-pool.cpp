#include "buffer-pool.h"

// Start of Frame class
Frame::Frame(): numKVPairs(0), pageId(""), next(nullptr) {}

Frame::Frame(KVPair *page, int numKVPairs, string pageId) : numBitsInBloomFilter(0), numKVPairs(numKVPairs), pageId(pageId), next(nullptr) {
    for (int i = 0; i < numKVPairs; i++) {
        this->page[i] = page[i];
    }
}

Frame::Frame(char *bitmap, int numBitsInBloomFilter, string pageId) : numBitsInBloomFilter(numBitsInBloomFilter), numKVPairs(0), pageId(pageId), next(nullptr) {
    memcpy(this->bitmap, bitmap, numBitsInBloomFilter);
}

void Frame::chain(Frame *newFrame) {
    Frame* curr = this;
    while (curr->next != nullptr) {
        curr = curr->next;
    }
    curr->next = newFrame;
}
// End of Frame Class

// Start of BufferPool class
BufferPool::BufferPool() : size(0) {
    for (int i = 0; i < BUFFER_POOL_SIZE; i++) {
        hashTable[i] = nullptr;
    }
};

BufferPool::~BufferPool() {
    for (int i = 0; i < BUFFER_POOL_SIZE; i++) {
        Frame *frame = hashTable[i];
        while (frame != nullptr) {
            Frame *temp = frame;
            frame = frame->getNext();
            delete temp;
        }
    }
}

int BufferPool::hash(string pageId) {
    uint64_t hashValue[2];
    MurmurHash3_x64_128(pageId.c_str(), pageId.length(), 0, hashValue);

    return hashValue[0] % BUFFER_POOL_SIZE;
}

void BufferPool::evictPage() {
    string page_to_evict = lruQueue.front();
    lruQueue.pop_front();

    int hashValue = hash(page_to_evict);
    Frame *curr = hashTable[hashValue];
    Frame *prev = nullptr;
    while (curr != nullptr) {
        if (curr->getPageId() == page_to_evict) {
            // set previous frame's next to current frame's next
            if (prev == nullptr) {
                // curr is the first frame in the chain
                hashTable[hashValue] = curr->getNext();
            }
            else {
                prev->setNext(curr->getNext());
            }
            delete curr;
            size -= 1;
            return;
        }
        prev = curr;
        curr = curr->getNext();
    }
}

void BufferPool::updateLRU(string pageId) {
    lruQueue.move_to_back(pageId);
}

/**
 * Put the page with the given pageId into the buffer pool
 * If the page is already in the buffer pool, update the LRU queue
 * If the buffer pool is at max capacity, evict the LRU page
 * Return 1 if the page is successfully added to the buffer pool
 */
int BufferPool::put(string pageId, KVPair *page, int numKVPairs) {
    int hashValue = hash(pageId);
    Frame *frame = new Frame(page, numKVPairs, pageId);

    // if buffer pool at max capacity, need to evict LRU page
    if (size == BUFFER_POOL_SIZE) {
        evictPage();
    }

    // add the page to the buffer pool
    if (hashTable[hashValue] != nullptr) {
        hashTable[hashValue]->chain(frame);
    }
    else {
        hashTable[hashValue] = frame;
    }

    // update LRU queue
    size += 1;
    updateLRU(pageId);

    return 1;
}

/** 
 * Get the page with the given pageId from the buffer pool
 * If the page is found:
 *  1) Update the LRU queue
 *  2) Copy the page into the outputPage
 *  3) Copy the number of KV pairs into numKVPairs
 *  4) Return 1
 * If the page is not found, return 0
 */
int BufferPool::get(string pageId, KVPair* outputPage, int *numKVPairs) {
    int hashValue = hash(pageId);
    Frame *frame = hashTable[hashValue];

    while (frame != nullptr) {
        if (frame->getPageId() == pageId) {
            updateLRU(pageId);
            
            int i = 0;
            while (i < frame->getNumKVPairs()) {
                outputPage[i] = frame->getPage()[i];
                i++;
            }

            *numKVPairs = frame->getNumKVPairs();
            
            return 1;
        }
        frame = frame->getNext();
    }

    return 0; // page not found
}

int BufferPool::putBfPage(string pageId, char *bitmap, int numBitsInBloomFilter) {
    int hashValue = hash(pageId);
    Frame *frame = new Frame(bitmap, numBitsInBloomFilter, pageId);

    // if buffer pool at max capacity, need to evict LRU page
    if (size == BUFFER_POOL_SIZE) {
        evictPage();
    }

    // add the page to the buffer pool
    if (hashTable[hashValue] != nullptr) {
        hashTable[hashValue]->chain(frame);
    }
    else {
        hashTable[hashValue] = frame;
    }

    // update LRU queue
    size += 1;
    updateLRU(pageId);

    return 1;
}

int BufferPool::getBfPage(string pageId, char*outputBitmap, int *numBitsInBloomFilter) {
    int hashValue = hash(pageId);
    Frame *frame = hashTable[hashValue];

    while (frame != nullptr) {
        if (frame->getPageId() == pageId) {
            updateLRU(pageId);
            memcpy(outputBitmap, frame->getBitmap(), frame->getNumBitsInBloomFilter());
            *numBitsInBloomFilter = frame->getNumBitsInBloomFilter();
            return 1;
        }
        frame = frame->getNext();
    }

    return 0; // page not found
}
// End of BufferPool class