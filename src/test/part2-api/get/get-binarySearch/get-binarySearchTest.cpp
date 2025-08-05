#include "get-binarySearchTest.h"

void testGetPart2BinarySearchKeyExistInSST() {
    memtable->clearTree();
    assert(memtable->getRoot() == nullptr);
    assert(memtable->getTreeSize() == 0);
    uint64_t key = 443;
    int64_t key_value = 443;
    currentOpenDB = "testGetKeyInSST";
    openDBPart2(currentOpenDB.c_str());
    // Starting the loop from 1 since key's cannot take value 0 in our implementation
    for (int64_t i = 1; i < MEMTABLE_SIZE + 2; ++i) {
        uint64_t key_put = static_cast<uint64_t>(i);
        int64_t value_put = static_cast<int64_t>(i);
        putPart2(key_put, value_put);
    }
    
    int64_t value = get_binarySearch(key);
    // cout << "Found vlaue in get = " << value << endl;
    assert(value == key_value);
    
     // clean up
    string dir = pwd();

    remove((dir + "/databases/db_names.txt").c_str());
    // Need to create db_names.txt again
    FILE *fp = openTextFile((dir + "/databases/db_names.txt").c_str(), "w");
    closeTextFile(fp);

    remove((dir + "/databases/" + currentOpenDB + "/file_names.txt").c_str());
    // Delete created SSTs
    for (const string &name : sstNames) {
        remove((dir + "/databases/" + currentOpenDB + "/" + name + ".bin").c_str());
    }
    rmdir((dir + "/databases/" + currentOpenDB).c_str());

    sstNames.clear();

}

void testGetPart2BinarySearchKeyNotExistInSST() {
    memtable->clearTree();
    assert(memtable->getRoot() == nullptr);
    assert(memtable->getTreeSize() == 0);
    uint64_t key = MEMTABLE_SIZE + 2;
    int64_t key_value = MEMTABLE_SIZE + 2;
    currentOpenDB = "testGetKeyNotInSST";
    openDBPart2(currentOpenDB.c_str());
    for (int64_t i = 1; i < MEMTABLE_SIZE + 2; ++i) {
        uint64_t key_put = static_cast<uint64_t>(i);
        int64_t value_put = static_cast<int64_t>(i);
        putPart2(key_put, value_put);
    }
    int64_t value = get_binarySearch(key);
    assert(value != key_value);
    
     // clean up
    string dir = pwd();

    remove((dir + "/databases/db_names.txt").c_str());
    // Need to create db_names.txt again
    FILE *fp = openTextFile((dir + "/databases/db_names.txt").c_str(), "w");
    closeTextFile(fp);

    remove((dir + "/databases/" + currentOpenDB + "/file_names.txt").c_str());
    // Delete created SSTs
    for (const string &name : sstNames) {
        remove((dir + "/databases/" + currentOpenDB + "/" + name + ".bin").c_str());
    }
    rmdir((dir + "/databases/" + currentOpenDB).c_str());

    sstNames.clear();

}


void getPart2BinarySearchTestAll() {
    cout << "RUNNING GET Binary Search API TESTS.." << endl << endl;

    cout << "Running GET API: Get key exists in SST..." << endl;
    testGetPart2BinarySearchKeyExistInSST();
    cout << "testGetPart2BinarySearchKeyExistInSST passed" << endl;

    cout << "Running GET API: Get key doesn't exists in SST..." << endl;
    testGetPart2BinarySearchKeyNotExistInSST();
    cout << "testGetPart2BinarySearchNotKeyExistInSST passed" << endl;

    cout << endl << "All Get API tests passed!" << endl << endl;
}