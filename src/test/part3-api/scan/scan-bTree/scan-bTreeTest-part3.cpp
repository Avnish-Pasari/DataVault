#include "scan-bTreeTest-part3.h"

void testScanInMemtablePart3BTree() {
    reset();

    openDBPart3("test-db-1");

    for (int i = 1; i <= 10; i++) {
        putPart3(i, i);
    }

    vector<KVPair> results = scan_bTree_part3(1, 10);
    assert(results.size() == 10);

    for (int i = 0; i < 10; i++) {
        assert(results[i].getKey() == i + 1);
        assert(results[i].getValue() == i + 1);
    }

    closeDBPart3();
}

void testScanInStoragePart3BTree() {
    reset();

    openDBPart3("test-db-2");

    for (int i = 1; i <= MEMTABLE_SIZE + 1; i++) {
        putPart3(i, i);
    }

    // Assert size of memtable is exactly one KV pair
    assert(memtable->getTreeSize() == 1);

    vector<KVPair> results = scan_bTree_part3(1, 5000);
    
    for (int i = 0; i < 5000; i++) {
        assert(results[i].getKey() == i + 1);
        assert(results[i].getValue() == i + 1);
    }

    closeDBPart3();
}

void testScanInMemtableAndStoragePart3BTree() {
    reset();

    openDBPart3("test-db-3");

    for (int i = 1; i <= MEMTABLE_SIZE + 100; i++) {
        putPart3(i, i);
    }

    assert(memtable->getTreeSize() == 100);

    vector<KVPair> results = scan_bTree_part3(1, MEMTABLE_SIZE + 100);
    
    for (int i = 0; i < MEMTABLE_SIZE + 100; i++) {
        assert(results[i].getKey() == i + 1);
        assert(results[i].getValue() == i + 1);
    }

    closeDBPart3();
}

void testScanEmptyRangePart3BTree() {
    reset();

    openDBPart3("test-db-4");

    for (int i = 3; i <= 5; i++) {
        putPart3(i, i);
    }

    vector<KVPair> results = scan_bTree_part3(6, 100);
    assert(results.size() == 0);

    results = scan_bTree_part3(1, 2);
    assert(results.size() == 0);

    closeDBPart3();
}

void testScanOneKeyDoesNotExistPart3BTree() {
    reset();

    openDBPart3("test-db-5");

    for (int i = MEMTABLE_SIZE / 2; i <= 2 * MEMTABLE_SIZE; i++) {
        putPart3(i, i);
    }

    vector<KVPair> results = scan_bTree_part3(1, MEMTABLE_SIZE / 2 + 10);
    assert(results.size() == 11);
    for (int i = 1; i <= 11; i++) {
        assert(results[i - 1].getKey() == MEMTABLE_SIZE / 2 + i - 1);
        assert(results[i - 1].getValue() == MEMTABLE_SIZE / 2 + i - 1);
    }

    results = scan_bTree_part3(2 * MEMTABLE_SIZE - 10, 2 * MEMTABLE_SIZE + 10);
    assert(results.size() == 11);
    for (int i = 2 * MEMTABLE_SIZE - 10; i <= 2 * MEMTABLE_SIZE; i += 1000) {
        assert(results[i - 2 * MEMTABLE_SIZE + 10].getKey() == i);
        assert(results[i - 2 * MEMTABLE_SIZE + 10].getValue() == i);
    }

    closeDBPart3();
}

void testScanBothKeysDoNotExistPart3BTree() {
    reset();

    openDBPart3("test-db-6");

    for (int i = 5; i <= 10; i++) {
        putPart3(i, i);
    }

    vector<KVPair> results = scan_bTree_part3(4, 11);
    assert(results.size() == 6);
    for (int i = 5; i <= 10; i++) {
        assert(results[i - 5].getKey() == i);
        assert(results[i - 5].getValue() == i);
    }

    closeDBPart3();
}

void testScanMostRecentKeyInOnlyMemtablePart3BTree() {
    reset();

    openDBPart3("test-db-7");

    for (int i = 1; i <= MEMTABLE_SIZE; i++) {
        putPart3(i, i);
    }

    putPart3(1, 2);

    vector<KVPair> results = scan_bTree_part3(1, MEMTABLE_SIZE);
    
    // Check if the most recent version of key 1 exists at the first index
    assert(results[0].getValue() == 2);

    // Check the other returned scan values
    for (int i = 0; i < MEMTABLE_SIZE; i++) {
        if (i + 1 == 1) {
            assert(results[i].getKey() == 1);
            assert(results[i].getValue() == 2);
            continue;
        }
        assert(results[i].getKey() == i + 1);
        assert(results[i].getValue() == i + 1);
    }

    closeDBPart3();
}

void testScanMostRecentKeyInMemtableAndStoragePart3BTree() {
    reset();

    openDBPart3("test-db-8");

    for (int i = 1; i <= MEMTABLE_SIZE + 5; i++) {
        putPart3(i, i);
    }
    // Put updated value for key in memtable and storage
    putPart3(1, 2);
    putPart3(MEMTABLE_SIZE + 2, 443);

    vector<KVPair> results = scan_bTree_part3(1, 2 * MEMTABLE_SIZE);
    
    for (int i = 0; i < MEMTABLE_SIZE + 5; i++) {
        if (i + 1 == 1) {
            assert(results[i].getKey() == 1);
            assert(results[i].getValue() == 2);
            continue;
        }
        else if (i + 1 == MEMTABLE_SIZE + 2) {
            assert(results[i].getKey() == i + 1);
            assert(results[i].getValue() == 443);
            continue;
        }
        assert(results[i].getKey() == i + 1);
        assert(results[i].getValue() == i + 1);
    }

    closeDBPart3();
}

void testScanMostRecentKeyInStoragePart3BTree() {
    reset();

    openDBPart3("test-db-9");

    // fill up memtable
    for (int i = 1; i <= MEMTABLE_SIZE; i++) {
        putPart3(i, i);
    }

    // force memtable flush with an update
    putPart3(MEMTABLE_SIZE, 443);

    vector<KVPair> results = scan_bTree_part3(1, MEMTABLE_SIZE);
    for (int i = 1; i < MEMTABLE_SIZE - 1; i++) {
        assert(results[i].getKey() == i + 1);
        assert(results[i].getValue() == i + 1);
    }

    assert(results[MEMTABLE_SIZE - 1].getKey() == MEMTABLE_SIZE);
    assert(results[MEMTABLE_SIZE - 1].getValue() == 443);

    closeDBPart3();
}

void testScanWithDeletedKeysPart3BTree() {
    reset();

    openDBPart3("test-db-10");

    for (int i = 1; i <= 50; i++) {
        putPart3(i, i);
    }
    
    // Delete the key 50
    deleteKey(50);

    vector<KVPair> results = scan_bTree_part3(1, 50); 

    for (int i = 0; i < results.size(); i++) {
        assert(results[i].getKey() == i + 1);
        assert(results[i].getValue() == i + 1);
    }
    
    // Assert that key 50 doesn't exist in output result
    assert(results[results.size() - 1].getKey() != 50);

    closeDBPart3();
}


void scanPart3BTreeTestAll() {
    cout << endl << "Running Part 3 API Scan (BTree Search) Tests..." << endl;

    cout << "Running testScanInMemtable..." << endl;
    testScanInMemtablePart3BTree();
    cout << "testScanInMemtable passed" << endl;
    cout << "Running testScanInStorage..." << endl;
    testScanInStoragePart3BTree();
    cout << "testScanInStorage passed" << endl;
    cout << "Running testScanInMemtableAndStorage..." << endl;
    testScanInMemtableAndStoragePart3BTree();
    cout << "testScanInMemtableAndStorage passed" << endl;

    cout << "Running testScanEmptyRange..." << endl;
    testScanEmptyRangePart3BTree();
    cout << "testScanEmptyRange passed" << endl;
    cout << "Running testScanOneKeyDoesNotExist..." << endl;
    testScanOneKeyDoesNotExistPart3BTree();
    cout << "testScanOneKeyDoesNotExist passed" << endl;
    cout << "Running testScanBothKeysDoNotExist..." << endl;
    testScanBothKeysDoNotExistPart3BTree();
    cout << "testScanBothKeysDoNotExist passed" << endl;

    cout << "Running testScanMostRecentKeyInOnlyMemtable..." << endl;
    testScanMostRecentKeyInOnlyMemtablePart3BTree();
    cout << "testScanMostRecentKeyInOnlyMemtable passed" << endl;
    cout << "Running testScanMostRecentKeyInMemtableAndStorage..." << endl;
    testScanMostRecentKeyInMemtableAndStoragePart3BTree();
    cout << "testScanMostRecentKeyInMemtableAndStorage passed" << endl;
    cout << "Running testScanMostRecentKeyInStorage..." << endl;
    testScanMostRecentKeyInStoragePart3BTree();
    cout << "testScanMostRecentKeyInStorage passed" << endl;
    cout << "Running testScanWithDeletedKeys..." << endl;
    testScanWithDeletedKeysPart3BTree();
    cout << "testScanWithDeletedKeys passed" << endl;

    cout << "All Scan (BTree Search) Tests Passed!" << endl << endl;
}