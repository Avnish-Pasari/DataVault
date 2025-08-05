#include "get-bTreeTest-part3.h"

void testKeyInMemtableBTree() {
    reset();

    openDBPart3("test-db-1");

    putPart3(1, 1);
    putPart3(2, 2);
    putPart3(3, 3);

    assert(get_bTree_part3(1) == 1);
    assert(get_bTree_part3(2) == 2);
    assert(get_bTree_part3(3) == 3);

    closeDBPart3();
}

void testKeyInStorageBTree() {
    reset();

    openDBPart3("test-db-2");
    
    for (int i = 1; i <= MEMTABLE_SIZE + 1; i++) {
        putPart3(i, i);
    }

    assert(memtable->getTreeSize() == 1);
    assert(get_bTree_part3(50) == 50);

    closeDBPart3();
}

void testKeyDoesNotExistBTree() {
    reset();

    openDBPart3("test-db-3");

    for (int i = 1; i <= 50; i++) {
        putPart3(i, i);
    }
    
    assert(get_bTree_part3(100) == INT64_MIN);

    closeDBPart3();
}

void testMostRecentKeyInOnlyMemtableBTree() {
    reset();

    openDBPart3("test-db-4");
    for (int i = 1; i <= MEMTABLE_SIZE; i++) {
        putPart3(i, i);
    }
    putPart3(1, 2);

    assert(get_bTree_part3(1) == 2);

    closeDBPart3();
}

void testMostRecentKeyInMemtableAndStorageBTree() {
    reset();

    openDBPart3("test-db-5");
    for (int i = 1; i <= MEMTABLE_SIZE; i++) {
        putPart3(i, i);
    }
    putPart3(1, 2);
    putPart3(MEMTABLE_SIZE + 1, MEMTABLE_SIZE + 1);

    assert(get_bTree_part3(1) == 2);
    assert(get_bTree_part3(MEMTABLE_SIZE + 1) == MEMTABLE_SIZE + 1);

    closeDBPart3();
}

void testMostRecentKeyInStorageBTree() {
    reset();

    openDBPart3("test-db-6");
    for (int i = 1; i <= MEMTABLE_SIZE; i++) {
        putPart3(i, i);
    }
    putPart3(1, 2);
    putPart3(MEMTABLE_SIZE + 1, MEMTABLE_SIZE + 1);

    assert(get_bTree_part3(1) == 2);
    assert(get_bTree_part3(MEMTABLE_SIZE + 1) == MEMTABLE_SIZE + 1);

    closeDBPart3();
}

void getPart3BTreeTestAll() {
    cout << endl << "Running Part 3 API Get (B-Tree) Tests..." << endl;

    cout << "Running testKeyInMemtableBTree..." << endl;
    testKeyInMemtableBTree();
    cout << "testKeyInMemtable passed" << endl;
    cout << "Running testKeyInStorageBTree..." << endl;
    testKeyInStorageBTree();
    cout << "testKeyInStorage passed" << endl;
    cout << "Running testKeyDoesNotExistBTree..." << endl;
    testKeyDoesNotExistBTree();
    cout << "testKeyDoesNotExist passed" << endl;

    cout << "Running testMostRecentKeyInOnlyMemtableBtree..." << endl;
    testMostRecentKeyInOnlyMemtableBTree();
    cout << "testMostRecentKeyInOnlyMemtable passed" << endl;
    cout << "Running testMostRecentKeyInMemtableAndStorageBTree..." << endl;
    testMostRecentKeyInMemtableAndStorageBTree();
    cout << "testMostRecentKeyInMemtableAndStorage passed" << endl;
    cout << "Running testMostRecentKeyInStorageBTree..." << endl;
    testMostRecentKeyInStorageBTree();
    cout << "testMostRecentKeyInStorage passed" << endl;

    cout << "All Part 3 API Get (B-Tree) Tests passed!" << endl << endl;
}