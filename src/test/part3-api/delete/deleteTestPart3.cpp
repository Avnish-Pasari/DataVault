#include "deleteTestPart3.h"

void testDeletesInMemtable() {
    reset();

    openDBPart3("testDeleteInMemtable");
    int size = 30;
    for (int i = 1; i < size; i++) {
        putPart3(i, i);
    }
    deleteKey(1);

    assert(memtable->getTreeSize() == size - 1);
    
    assert(get_binarySearch_part3(1) == INT64_MIN);
    assert(get_bTree_part3(1) == INT64_MIN);

    closeDBPart3();
}

void testDeletesInStorage() {
    reset();

    openDBPart3("testDeletesInStorage");
    for (int i = 1; i <= MEMTABLE_SIZE + 1; i++) {
        putPart3(i, i);
    }
    deleteKey(1);

    assert(get_binarySearch_part3(1) == INT64_MIN);
    assert(get_bTree_part3(1) == INT64_MIN);

    closeDBPart3();
}

void testDeletesAndReinsertion() {
    reset();

    openDBPart3("testDeletesAndReinsertion");
    for (int i = 1; i <= MEMTABLE_SIZE; i++) {
        putPart3(i, i);
    }
    deleteKey(1);

    assert(get_binarySearch_part3(1) == INT64_MIN);
    assert(get_bTree_part3(1) == INT64_MIN);

    putPart3(1, 2);

    assert(get_binarySearch_part3(1) == 2);
    assert(get_bTree_part3(1) == 2);

    closeDBPart3();
}

void deletePart3TestAll() {
    cout << endl << "Running Part 3 API Delete Tests..." << endl;

    cout << "Running testDeletesInMemtable..." << endl;
    testDeletesInMemtable();
    cout << "testDeletesInMemtable passed" << endl;
    cout << "Running testDeletesOutsideMemtable..." << endl;
    testDeletesInStorage();
    cout << "testDeletesOutsideMemtable passed" << endl;
    cout << "Running testDeletesAndReinsertion..." << endl;
    testDeletesAndReinsertion();
    cout << "testDeletesAndReinsertion passed" << endl;

    cout << "All Part 3 API Delete Tests passed!" << endl << endl;
}