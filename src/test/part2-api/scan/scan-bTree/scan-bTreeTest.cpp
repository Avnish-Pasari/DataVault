#include "scan-bTreeTest.h"

void testScanPart2BTreeKeyExistInSST() {
    memtable->clearTree();
    assert(memtable->getRoot() == nullptr);
    assert(memtable->getTreeSize() == 0);
    uint64_t key1 = 343;
    int64_t key1_value = 343;

    uint64_t key2 = 443;
    int64_t key2_value = 443;
    currentOpenDB = "testScanKeyInSST";
    openDBPart2(currentOpenDB.c_str());
    // Starting the loop from 1 since key's cannot take value 0 in our implementation
    for (int64_t i = 1; i < MEMTABLE_SIZE + 2; ++i) {
        uint64_t key_put = static_cast<uint64_t>(i);
        int64_t value_put = static_cast<int64_t>(i);
        putPart2(key_put, value_put);
    }
    
    vector<KVPair> foundPairs = scan_bTree(key1, key2);
    int idx = 0;
    for (uint64_t i = key1; i < key2; i++) {
        assert(i == foundPairs[idx].getValue());
        idx++;
    }
    
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

void testScanPart2BTreeAcrossMemtableAndSST() {
    memtable->clearTree();
    assert(memtable->getRoot() == nullptr);
    assert(memtable->getTreeSize() == 0);
    uint64_t key1 = 1;
    int64_t key1_value = 1;

    uint64_t key2 = MEMTABLE_SIZE + 3;
    int64_t key2_value = MEMTABLE_SIZE + 3;
    currentOpenDB = "testScanKeyInSST";
    openDBPart2(currentOpenDB.c_str());
    // Starting the loop from 1 since key's cannot take value 0 in our implementation
    for (int64_t i = 1; i < MEMTABLE_SIZE + 8; ++i) {
        uint64_t key_put = static_cast<uint64_t>(i);
        int64_t value_put = static_cast<int64_t>(i);
        putPart2(key_put, value_put);
    }
    
    vector<KVPair> foundPairs = scan_bTree(key1, key2);
    int idx = 0;
    for (uint64_t i = key1; i < key2; i++) {
        assert(i == foundPairs[idx].getValue());
        idx++;
    }
    
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

void testScanPart2BTreeAcrossSSTs() {
    memtable->clearTree();
    assert(memtable->getRoot() == nullptr);
    assert(memtable->getTreeSize() == 0);
    uint64_t key1 = MEMTABLE_SIZE - 3;
    int64_t key1_value = MEMTABLE_SIZE - 3;

    uint64_t key2 = MEMTABLE_SIZE + 3;
    int64_t key2_value = MEMTABLE_SIZE + 3;
    currentOpenDB = "testScanKeyInSSTs";
    openDBPart2(currentOpenDB.c_str());
    // Starting the loop from 1 since key's cannot take value 0 in our implementation
    for (int64_t i = 1; i < 2 * MEMTABLE_SIZE + 5; ++i) {
        uint64_t key_put = static_cast<uint64_t>(i);
        int64_t value_put = static_cast<int64_t>(i);
        putPart2(key_put, value_put);
    }
    
    vector<KVPair> foundPairs = scan_bTree(key1, key2);
    int idx = 0;
    for (uint64_t i = key1; i < key2; i++) {
        assert(i == foundPairs[idx].getValue());
        idx++;
    }
    
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

void scanPart2BTreeTestAll() {
    cout << "RUNNING SCAN B-Tree API TESTS.." << endl << endl;

    cout << "Running SCAN API: Scan keys exists in SST..." << endl;
    testScanPart2BTreeKeyExistInSST();
    cout << "testScanPart2BTreeKeyExistInSST passed" << endl;

    cout << "Running SCAN API: Scan keys across memtable and SST..." << endl;
    testScanPart2BTreeAcrossMemtableAndSST();
    cout << "testScanPart2BTreeAcrossMemtableAndSST passed" << endl;

    cout << "Running SCAN API: Scan keys across SSTs..." << endl;
    testScanPart2BTreeAcrossSSTs();
    cout << "testScanPart2BTreeAcrossSSTs passed" << endl;

    cout << endl << "All SCAN B-Tree Search API tests passed!" << endl << endl;
}