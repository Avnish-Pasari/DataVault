#include "get-binarySearchTest-part3.h"

void testKeyInMemtable() {
    reset();

    openDBPart3("test-db-1");

    putPart3(1, 1);
    putPart3(2, 2);
    putPart3(3, 3);

    assert(get_binarySearch_part3(1) == 1);
    assert(get_binarySearch_part3(2) == 2);
    assert(get_binarySearch_part3(3) == 3);

    closeDBPart3();
}

void testKeyInStorage() {
    reset();

    openDBPart3("test-db-2");
    
    for (int i = 1; i <= MEMTABLE_SIZE + 1; i++) {
        putPart3(i, i);
    }

    assert(memtable->getTreeSize() == 1);
    
    assert(get_binarySearch_part3(50) == 50);

    closeDBPart3();
}

void testKeyDoesNotExist() {
    reset();

    openDBPart3("test-db-3");

    for (int i = 1; i <= 50; i++) {
        putPart3(i, i);
    }

    assert(get_binarySearch_part3(100) == INT64_MIN);

    closeDBPart3();
}

void testMostRecentKeyInOnlyMemtable() {
    reset();

    openDBPart3("test-db-4");

    putPart3(1, 1);
    putPart3(1, 2);

    assert(get_binarySearch_part3(1) == 2);

    closeDBPart3();
}

void testMostRecentKeyInMemtableAndStorage() {
    reset();

    openDBPart3("test-db-5");

    for (int i = 1; i <= MEMTABLE_SIZE; i++) {
        putPart3(i, i);
    }
    putPart3(1, 2);
    putPart3(MEMTABLE_SIZE + 1, MEMTABLE_SIZE + 1);

    assert(get_binarySearch_part3(1) == 2);
    assert(get_binarySearch_part3(MEMTABLE_SIZE + 1) == MEMTABLE_SIZE + 1);

    closeDBPart3();
}

void testMostRecentKeyInStorage() {
    reset();

    openDBPart3("test-db-5");

    for (int i = 1; i <= MEMTABLE_SIZE * 4; i++) {
        putPart3(i, i);
    }
    putPart3(1, 2);
    putPart3(MEMTABLE_SIZE + 1, MEMTABLE_SIZE + 1);

    assert(get_binarySearch_part3(1) == 2);
    assert(get_binarySearch_part3(MEMTABLE_SIZE + 1) == MEMTABLE_SIZE + 1);

    closeDBPart3();
}

void getPart3BinarySearchTestAll() {
    cout << endl << "Running Part 3 API Get (Binary Search) Tests..." << endl;

    cout << "Running testKeyInMemtable..." << endl;
    testKeyInMemtable();
    cout << "testKeyInMemtable passed" << endl;
    cout << "Running testKeyInStorage..." << endl;
    testKeyInStorage();
    cout << "testKeyInStorage passed" << endl;
    cout << "Running testKeyDoesNotExist..." << endl;
    testKeyDoesNotExist();
    cout << "testKeyDoesNotExist passed" << endl;

    cout << "Running testMostRecentKeyInOnlyMemtable..." << endl;
    testMostRecentKeyInOnlyMemtable();
    cout << "testMostRecentKeyInOnlyMemtable passed" << endl;
    cout << "Running testMostRecentKeyInMemtableAndStorage..." << endl;
    testMostRecentKeyInMemtableAndStorage();
    cout << "testMostRecentKeyInMemtableAndStorage passed" << endl;
    cout << "Running testMostRecentKeyInStorage..." << endl;
    testMostRecentKeyInStorage();
    cout << "testMostRecentKeyInStorage passed" << endl;

    cout << "All Part 3 API Get (Binary Search) Tests passed!" << endl << endl;
}