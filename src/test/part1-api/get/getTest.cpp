#include "../get/getTest.h"

void testGetKeyExistInMemtable() {
    memtable->clearTree();
    assert(memtable->getRoot() == nullptr);
    assert(memtable->getTreeSize() == 0);
    uint64_t key = 5;
    int64_t key_value = 5;
    for (int64_t i = 1; i < 10; ++i) {
        put(i, i);
    }
    int64_t value = get(key);
    assert(value == key_value);
}
void testGetKeyNotExistInMemtable() {
    memtable->clearTree();
    assert(memtable->getRoot() == nullptr);
    assert(memtable->getTreeSize() == 0);
    uint64_t key = 25;
    int64_t key_value = 25;
    for (int64_t i = 1; i < 10; ++i) {
        put(i, i);
    }
    int64_t value = get(key);
    assert(value != key_value);
}
void testGetKeyExistInSST() {
    memtable->clearTree();
    assert(memtable->getRoot() == nullptr);
    assert(memtable->getTreeSize() == 0);
    uint64_t key = 18;
    int64_t key_value = 18;
    currentOpenDB = "testGetKeyInSST";
    openDBPart1(currentOpenDB.c_str());
    for (int64_t i = 1; i < MEMTABLE_SIZE + 2; ++i) {
        put(i, i);
    }
    int64_t value = get(key);
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

void testGetKeyNotExistInSST() {
    memtable->clearTree();
    assert(memtable->getRoot() == nullptr);
    assert(memtable->getTreeSize() == 0);
    uint64_t key = MEMTABLE_SIZE + 2;
    int64_t key_value = MEMTABLE_SIZE + 2;
    currentOpenDB = "testGetKeyNotInSST";
    openDBPart1(currentOpenDB.c_str());
    for (int64_t i = 1; i < MEMTABLE_SIZE + 2; ++i) {
        put(i, i);
    }
    int64_t value = get(key);
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

void getTestAll() {
    cout << "RUNNING GET API TESTS.." << endl << endl;

    cout << "Running GET API: Get key exists in memtable..." << endl;
    testGetKeyExistInMemtable();
    cout << "testGetKeyExistInMemtable passed" << endl;

    cout << "Running GET API: Get key doesn't exists in memtable..." << endl;
    testGetKeyNotExistInMemtable();
    cout << "testGetKeyNotExistInMemtable passed" << endl;

    cout << "Running GET API: Get key exists in SST..." << endl;
    testGetKeyExistInSST();
    cout << "testGetKeyExistInSST passed" << endl;

    cout << "Running GET API: Get key doesn't exists in SST..." << endl;
    testGetKeyNotExistInSST();
    cout << "testGetKeyExistInSST passed" << endl;

    cout << endl << "All Get API tests passed!" << endl << endl;
}