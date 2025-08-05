#include "bufferPoolTest.h"

void testBufferPoolInitialization() {
    BufferPool bufferPool;
    assert(bufferPool.getSize() == 0);
    for (int i = 0; i < BUFFER_POOL_SIZE; i++) {
        assert(bufferPool.getHashTable()[i] == nullptr);
    }
}

void testBufferPoolSingleInsertionAndRetrieval() {
    BufferPool bufferPool;
    
    KVPair *page = simulateGetApi();
    char *bitmapPage = simulateBfGetApi();

    bufferPool.put("SST_1.bin-1", page, KV_PAIRS_PER_PAGE);
    assert(bufferPool.getSize() == 1);
    assert(bufferPool.getLRUQueue().getSize() == 1);
    assert(bufferPool.getLRUQueue().front() == "SST_1.bin-1");

    bufferPool.putBfPage("BF_1.bin-1", bitmapPage, PAGE_SIZE);
    assert(bufferPool.getSize() == 2);
    assert(bufferPool.getLRUQueue().getSize() == 2);
    assert(bufferPool.getLRUQueue().front() == "SST_1.bin-1");
    assert(bufferPool.getLRUQueue().back() == "BF_1.bin-1");

    KVPair kvPairs[KV_PAIRS_PER_PAGE];
    int numKVPairs;
    int ret = bufferPool.get("SST_1.bin-1", kvPairs, &numKVPairs);

    assert(numKVPairs == KV_PAIRS_PER_PAGE);
    for (int i = 0; i < KV_PAIRS_PER_PAGE; i++) {
        assert(kvPairs[i].getKey() == i);
        assert(kvPairs[i].getValue() == i);
    }
    assert(ret == 1);

    int ret2 = bufferPool.get("SST_1.bin-2", kvPairs, &numKVPairs);
    assert(ret2 == 0);

    char bitmap[PAGE_SIZE];
    int numBitsInBloomFilter;
    int ret3 = bufferPool.getBfPage("BF_1.bin-1", bitmap, &numBitsInBloomFilter);

    assert(numBitsInBloomFilter == PAGE_SIZE);
    for (int i = 0; i < PAGE_SIZE; i++) {
        assert(bitmap[i] == i % 2);
    }
    assert(ret3 == 1);

    cleanup(page);
    cleanup(bitmapPage);
}

void testBufferPoolMultipleInsertions() {
    BufferPool bufferPool;
    KVPair *page1 = simulateGetApi();
    KVPair *page2 = simulateGetApi();
    KVPair *page3 = simulateGetApi();
    char *bitmapPage = simulateBfGetApi();
    char *bitmapPage2 = simulateBfGetApi();
    char *bitmapPage3 = simulateBfGetApi();

    bufferPool.put("SST_1.bin-1", page1, KV_PAIRS_PER_PAGE);
    bufferPool.putBfPage("BF_1.bin-1", bitmapPage, PAGE_SIZE);
    bufferPool.put("SST_1.bin-2", page2, KV_PAIRS_PER_PAGE);
    bufferPool.putBfPage("BF_1.bin-2", bitmapPage2, PAGE_SIZE);
    bufferPool.put("SST_1.bin-3", page3, KV_PAIRS_PER_PAGE);
    bufferPool.putBfPage("BF_1.bin-3", bitmapPage3, PAGE_SIZE);

    assert(bufferPool.getSize() == 6);
    assert(bufferPool.getLRUQueue().getSize() == 6);
    assert(bufferPool.getLRUQueue().front() == "SST_1.bin-1");
    assert(bufferPool.getLRUQueue().back() == "BF_1.bin-3");
    assert(bufferPool.getLRUQueue().getHead()->getNext()->getPageId() == "BF_1.bin-1");

    cleanup(page1);
    cleanup(page2);
    cleanup(page3);
    cleanup(bitmapPage);
    cleanup(bitmapPage2);
    cleanup(bitmapPage3);
}

void testBufferPoolCollisions() {
    BufferPool bufferPool;
    KVPair *page1 = simulateGetApi();
    KVPair *page2 = simulateGetApi();
    page2[0] = KVPair(-1, -1);

    bufferPool.put("SST_1.bin-1", page1, KV_PAIRS_PER_PAGE);
    bufferPool.put("SST_1.bin-2", page2, KV_PAIRS_PER_PAGE);

    assert(bufferPool.getSize() == 2);
    assert(bufferPool.getLRUQueue().getSize() == 2);

    KVPair kvPairs[KV_PAIRS_PER_PAGE];
    int numKVPairs;
    int ret = bufferPool.get("SST_1.bin-1", kvPairs, &numKVPairs);

    assert(ret == 1);
    for (int i = 0; i < KV_PAIRS_PER_PAGE; i++) {
        assert(kvPairs[i].getKey() == i);
        assert(kvPairs[i].getValue() == i);
    }

    int ret2 = bufferPool.get("SST_1.bin-2", kvPairs, &numKVPairs);
    assert(ret2 == 1);
    assert(kvPairs[0].getKey() == -1);
    assert(kvPairs[0].getValue() == -1);
    for (int i = 1; i < KV_PAIRS_PER_PAGE; i++) {
        assert(kvPairs[i].getKey() == i);
        assert(kvPairs[i].getValue() == i);
    }

    cleanup(page1);
    cleanup(page2);
}

void testBufferPoolEvictionNoCollisions() {
    BufferPool bufferPool;
    
    KVPair *page = simulateGetApi();
    for (int i = 0; i < BUFFER_POOL_SIZE / 8; i++) {
        for (int j = 0; j < 8; j++) {
            bufferPool.put("SST_" + to_string(i) + ".bin-" + to_string(j), page, KV_PAIRS_PER_PAGE);
        }
    }

    assert(bufferPool.getSize() == BUFFER_POOL_SIZE);

    KVPair kvPairs[KV_PAIRS_PER_PAGE];
    int numKVPairs;
    int ret = bufferPool.get("SST_0.bin-0", kvPairs, &numKVPairs);

    assert(ret == 1);
    for (int i = 0; i < KV_PAIRS_PER_PAGE; i++) {
        assert(kvPairs[i].getKey() == i);
        assert(kvPairs[i].getValue() == i);
    }

    // check that LRU queue works as expected
    assert(bufferPool.getLRUQueue().back() == "SST_0.bin-0");
    assert(bufferPool.getLRUQueue().front() != "SST_0.bin-0");
    assert(bufferPool.getSize() == BUFFER_POOL_SIZE);

    // check that the first page is evicted
    assert(bufferPool.getLRUQueue().front() == "SST_0.bin-1");
    bufferPool.put("SST_99.bin-99", kvPairs, KV_PAIRS_PER_PAGE);
    assert(bufferPool.getSize() == BUFFER_POOL_SIZE);
    assert(bufferPool.get("SST_0.bin-0", kvPairs, &numKVPairs) == 1);
    assert(bufferPool.get("SST_0.bin-1", kvPairs, &numKVPairs) == 0);

    for (int i = 0; i < KV_PAIRS_PER_PAGE; i++) {
        assert(kvPairs[i].getKey() == i);
        assert(kvPairs[i].getValue() == i);
    }

    cleanup(page);
}

void testBufferPoolEvictionWithCollisions() {
    BufferPool bufferPool;

    for (int i = 0; i < BUFFER_POOL_SIZE / 8; i++) {
        KVPair page[KV_PAIRS_PER_PAGE] = {KVPair(i, i)};
        for (int j = 0; j < 8; j++) {
            bufferPool.put("SST_" + to_string(i) + ".bin-" + to_string(j), page, KV_PAIRS_PER_PAGE);
        }
    }

    KVPair kvPairs[KV_PAIRS_PER_PAGE];
    int numKVPairs;
    bufferPool.get("SST_0.bin-0", kvPairs, &numKVPairs);

    bufferPool.put("SST_99.bin-99", kvPairs, KV_PAIRS_PER_PAGE);

    assert(bufferPool.get("SST_0.bin-1", kvPairs, &numKVPairs) == 0);

    Frame *frame = bufferPool.getHashTable()[0];
    assert(frame->getPageId() == "SST_0.bin-0");
    assert(frame->getNext()->getPageId() == "SST_0.bin-2");
}

void testBufferPoolChainingStructure() {
    BufferPool bufferPool;
    KVPair *page1 = simulateGetApi();
    KVPair *page2 = simulateGetApi();
    KVPair *page3 = simulateGetApi();

    bufferPool.put("SST_1.bin-1", page1, KV_PAIRS_PER_PAGE);
    bufferPool.put("SST_1.bin-2", page2, KV_PAIRS_PER_PAGE);
    bufferPool.put("SST_1.bin-3", page3, KV_PAIRS_PER_PAGE);

    Frame* frame = bufferPool.getHashTable()[0];
    assert(frame->getPageId() == "SST_1.bin-1");
    assert(frame->getNext()->getPageId() == "SST_1.bin-2");
    assert(frame->getNext()->getNext()->getPageId() == "SST_1.bin-3");

    cleanup(page1);
    cleanup(page2);
    cleanup(page3);
}


void bufferPoolTestAll() {
    cout << "Running Buffer Pool tests..." << endl;

    cout << "Running testBufferPoolInitialization..." << endl;
    testBufferPoolInitialization();
    cout << "testBufferPoolInitialization passed" << endl;
    cout << "Running testBufferPoolSingleInsertionAndRetrieval..." << endl;
    testBufferPoolSingleInsertionAndRetrieval();
    cout << "testBufferPoolSingleInsertionAndRetrieval passed" << endl;
    cout << "Running testBufferPoolMultipleInsertions..." << endl;
    testBufferPoolMultipleInsertions();
    cout << "testBufferPoolMultipleInsertions passed" << endl;

    cout << "Running testBufferPoolEvictionNoCollisions..." << endl;
    testBufferPoolEvictionNoCollisions();
    cout << "testBufferPoolEvictionNoCollisions passed" << endl;

    // NOTE: To run these tests, set the hash function in buffer-pool.cpp to always return 0
    // cout << "Running testBufferPoolCollisions" << endl
    // testBufferPoolCollisions();
    // cout << "testBufferPoolCollisions passed" << endl;
    // cout << "Running testBufferPoolEvictionWithCollisions"
    // testBufferPoolEvictionWithCollisions();
    // cout << "testBufferPoolEvictionWithCollisions passed" << endl;
    
    cout << "Buffer Pool tests passed!" << endl;
}