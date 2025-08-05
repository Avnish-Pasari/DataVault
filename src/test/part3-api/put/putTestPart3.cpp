#include "putTestPart3.h"

void testFlushToDiskNoCompactionWithoutClosingDB() {
    reset();

    openDBPart3("test-db-1");

    for (int i = 1; i <= MEMTABLE_SIZE + 1; i++) {
        putPart3(i, i);
    }

    assert(system("test -f ../databases/test-db-1/SST1.bin") != 0);
    assert(system("test -f ../databases/test-db-1/SST2.bin") == 0);
    assert(lsmNames.size() == 2);
    assert(memtable->getTreeSize() > 0);

    for (int i = 1; i <= MEMTABLE_SIZE + 1; i += 1000) {
        assert(get_binarySearch_part3(i) == i);
        assert(get_bTree_part3(i) == i);
    }
    assert(get_binarySearch_part3(MEMTABLE_SIZE + 2) == INT64_MIN);
    assert(get_bTree_part3(MEMTABLE_SIZE + 2) == INT64_MIN);

    closeDBPart3();
}

void testFlushToDiskWithCompactionWithoutClosingDB() {
    reset();

    openDBPart3("test-db-2");

    // force compaction
    for (int i = 1; i <= MEMTABLE_SIZE; i++) {
        putPart3(i, i);
    }
    putPart3(MEMTABLE_SIZE + 1, MEMTABLE_SIZE + 1);
    for (int i = MEMTABLE_SIZE + 2; i <= MEMTABLE_SIZE * 2; i++) {
        putPart3(i, i);
    }
    putPart3(MEMTABLE_SIZE * 2 + 1, MEMTABLE_SIZE * 2 + 1);

    assert(lsmNames.size() == 3);
    assert(memtable->getTreeSize() > 0);

    for (int i = 1; i <= MEMTABLE_SIZE * 2 + 1; i += 1000) {
        assert(get_binarySearch_part3(i) == i);
        assert(get_bTree_part3(i) == i);
    }
    assert(get_binarySearch_part3(MEMTABLE_SIZE * 2 + 2) == INT64_MIN);
    assert(get_bTree_part3(MEMTABLE_SIZE * 2 + 2) == INT64_MIN);

    closeDBPart3();
}

void testPutInsertionIntoMemtable() {
    reset();

    openDBPart3("test-db-3");

    for (int i = 1; i <= MEMTABLE_SIZE; i++) {
        putPart3(i, i);
    }
    assert(memtable->getTreeSize() == MEMTABLE_SIZE);

    for (int i = 1; i <= MEMTABLE_SIZE; i += 1000) {
        assert(get_binarySearch_part3(i) == i);
        assert(get_bTree_part3(i) == i);
    }
    assert(get_binarySearch_part3(MEMTABLE_SIZE + 1) == INT64_MIN);
    assert(get_bTree_part3(MEMTABLE_SIZE + 1) == INT64_MIN);

    closeDBPart3();
}

void testPutLargeDataInsertions() {
    reset();

    openDBPart3("test-db-4");

    for (int i = 1; i <= MEMTABLE_SIZE * 16 + 1; i++) {
        putPart3(i, i);
    }

    for (int i = 1; i <= MEMTABLE_SIZE * 16 + 1; i += 1000) {
        assert(get_binarySearch_part3(i) == i);
        assert(get_bTree_part3(i) == i);
    }
    assert(get_binarySearch_part3(MEMTABLE_SIZE * 16 + 2) == INT64_MIN);
    assert(get_bTree_part3(MEMTABLE_SIZE * 16 + 2) == INT64_MIN);

    closeDBPart3();
}

void testUpdatesInMemtable() {
    reset();

    openDBPart3("test-db-5");

    for (int i = 1; i < MEMTABLE_SIZE; i++) {
        putPart3(i, i);
    }
    putPart3(1, 2);
    
    assert(memtable->getTreeSize() == MEMTABLE_SIZE - 1);
    vector<KVPair> kvPairs = memtable->inorderTraversal();
    
    int count = 0;
    for (int i = 0; i < kvPairs.size(); i++) {
        if (kvPairs[i].getKey() == 1) {
            count++;
        }
    }
    assert(count == 1);
    assert(memtable->get(1)->getValue() == 2);
    
    assert(get_binarySearch_part3(1) == 2);
    assert(get_bTree_part3(1) == 2);

    closeDBPart3();
}

void testUpdatesOutsideMemtable() {
    reset();

    openDBPart3("test-db-6");

    for (int i = 1; i <= MEMTABLE_SIZE + 1; i++) {
        putPart3(i, i);
    }
    putPart3(1, 2);

    // check that the old value is present in the sst
    FILE *fp = openBinaryFile("../databases/test-db-6/SST2.bin", "rb");
    KVPair page[KV_PAIRS_PER_PAGE];
    readPageFromBinaryFile(fp, 0, page);
    assert(page[0].getValue() == 1);
    closeBinaryFile(fp);

    // check that the new value is returned
    assert(memtable->get(1)->getValue() == 2);
    assert(get_binarySearch_part3(1) == 2);
    assert(get_bTree_part3(1) == 2);

    closeDBPart3();
}

void testUpdatesDeleteOldKeyValuePair() {
    reset();

    openDBPart3("test-db-7");

    // insert key and flush to sst
    for (int i = 1; i <= MEMTABLE_SIZE; i++) {
        putPart3(i, i);
    }
    // update key and flush to sst
    putPart3(1, 2);
    for (int i = 1; i <= MEMTABLE_SIZE; i++) {
        putPart3(MEMTABLE_SIZE + i, i);
    }

    // check that the old value is no longer present in the sst
    FILE *fp = openBinaryFile("../databases/test-db-7/SST4.bin", "rb");
    KVPair page[KV_PAIRS_PER_PAGE];
    readPageFromBinaryFile(fp, 0, page);
    assert(page[0].getValue() == 2);
    closeBinaryFile(fp);

    // check that the new value is returned
    assert(get_binarySearch_part3(1) == 2);
    assert(get_bTree_part3(1) == 2);

    closeDBPart3();
}


void putPart3TestAll() {
    cout << endl << "Running Part 3 API Put Tests..." << endl;

    cout << "Running testFlushToDiskNoCompactionWithoutClosingDB..." << endl;
    testFlushToDiskNoCompactionWithoutClosingDB();
    cout << "testFlushToDiskNoCompactionWithoutClosingDB passed" << endl;
    cout << "Running testFlushToDiskWithCompactionWithoutClosingDB..." << endl;
    testFlushToDiskWithCompactionWithoutClosingDB();
    cout << "testFlushToDiskWithCompactionWithoutClosingDB passed" << endl;

    cout << "Running testPutInsertionIntoMemtable..." << endl;
    testPutInsertionIntoMemtable();
    cout << "testPutInsertionIntoMemtable passed" << endl;
    cout << "Running testPutLargeDataInsertions. This may take a while." << endl;
    testPutLargeDataInsertions();
    cout << "testPutLargeDataInsertions passed" << endl;

    cout << "Running testUpdatesInMemtable..." << endl;
    testUpdatesInMemtable();
    cout << "testUpdatesInMemtable passed" << endl;
    cout << "Running testUpdatesOutsideMemtable..." << endl;
    testUpdatesOutsideMemtable();
    cout << "testUpdatesOutsideMemtable passed" << endl;
    cout << "Running testUpdatesDeleteOldKeyValuePair..." << endl;
    testUpdatesDeleteOldKeyValuePair();
    cout << "testUpdatesDeleteOldKeyValuePair passed" << endl;

    cout << "All Part 3 API Put Tests passed!" << endl << endl;
}