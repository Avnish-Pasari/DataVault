#include "./scanTest.h"

void testScanInMemtable() {
    memtable->clearTree();
    assert(memtable->getRoot() == nullptr);
    assert(memtable->getTreeSize() == 0);

    int iteration = 1;
    uint64_t key;
    int64_t value;
    while (iteration < 10) {
        key = static_cast<uint64_t>(iteration);
        value = static_cast<int64_t>(iteration);
        put(key, value);
        iteration++;
    }
    iteration = 1;
    int64_t key_value_check;
    vector<KVPair> kvPairs = scan(1, 10);
    sort(kvPairs.begin(), kvPairs.end());
    while (iteration < 10) {
        key = static_cast<uint64_t>(iteration);
        key_value_check = static_cast<int64_t>(iteration);
        assert(key_value_check == kvPairs[iteration - 1].getValue());
        iteration++;
    }
}

// Test to scan where key2 range is out of scan range
void testScanInMemtableOutsideRange() {
    memtable->clearTree();
    assert(memtable->getRoot() == nullptr);
    assert(memtable->getTreeSize() == 0);

    int iteration = 1;
    uint64_t key;
    int64_t value;
    while (iteration < 10) {
        key = static_cast<uint64_t>(iteration);
        value = static_cast<int64_t>(iteration);
        put(key, value);
        iteration++;
    }
    iteration = 1;
    int64_t key_value_check;
    // Key 20 does not exist in the range of insertions made in above loop
    vector<KVPair> kvPairs = scan(1, 20);
    sort(kvPairs.begin(), kvPairs.end());
    for (int i = 1; i < 10; ++i) {
        key = static_cast<uint64_t>(i);
        key_value_check = static_cast<int64_t>(i);
        assert(key_value_check == kvPairs[i - 1].getValue());
    }
}

void testScanInSST() {
    memtable->clearTree();
    assert(memtable->getRoot() == nullptr);
    assert(memtable->getTreeSize() == 0);

    currentOpenDB = "testScanInSST";
    openDBPart1(currentOpenDB.c_str());

    int iteration = 1;
    uint64_t key;
    int64_t value;
    while (iteration < MEMTABLE_SIZE + 1) {
        key = static_cast<uint64_t>(iteration);
        value = static_cast<int64_t>(iteration);
        put(key, value);
        iteration++;
    }
    // Adding another KV pair so we force a flush to disk
    put(MEMTABLE_SIZE + 5, MEMTABLE_SIZE + 5);

    int64_t key_value_check;
    vector<KVPair> kvPairs = scan(1, MEMTABLE_SIZE);
    sort(kvPairs.begin(), kvPairs.end());
    for(int i = 1; i < MEMTABLE_SIZE + 1; ++i) {
        key = static_cast<uint64_t>(i);
        key_value_check = key = static_cast<int64_t>(i);
        assert(key_value_check == kvPairs[i - 1].getValue());
    }
    // clean up
    string dir = pwd();

    remove((dir + "/databases/db_names.txt").c_str());
    // Need to create db_names.txt again
    FILE* fp = openTextFile((dir + "/databases/db_names.txt").c_str(), "w");
    closeTextFile(fp);

    remove((dir + "/databases/" + currentOpenDB + "/file_names.txt").c_str());
    // Delete created SSTs
    for (const string &name : sstNames) {
        remove((dir + "/databases/" + currentOpenDB + "/" + name + ".bin").c_str());
    }
    rmdir((dir + "/databases/" + currentOpenDB).c_str());

    sstNames.clear();
}

void testScanAcrossMemtableAndSST() {
    memtable->clearTree();
    assert(memtable->getRoot() == nullptr);
    assert(memtable->getTreeSize() == 0);

    currentOpenDB = "testScanMemtableAndSST";
    openDBPart1(currentOpenDB.c_str());

    int iteration = 1;
    uint64_t key;
    int64_t value;
    while (iteration < MEMTABLE_SIZE + 5) {
        key = static_cast<uint64_t>(iteration);
        value = static_cast<int64_t>(iteration);
        put(key, value);
        iteration++;
    }
    put(583924, 583924);

    int64_t key_value_check;
    vector<KVPair> kvPairs = scan(1, MEMTABLE_SIZE + 3);
    sort(kvPairs.begin(), kvPairs.end());
    for(int i = 1; i < MEMTABLE_SIZE + 4; ++i) {
        key = static_cast<uint64_t>(i);
        key_value_check = key = static_cast<int64_t>(i);
        assert(key_value_check == kvPairs[i - 1].getValue());
    }
     // clean up
    string dir = pwd();

    remove((dir + "/databases/db_names.txt").c_str());
    // Need to create db_names.txt again
    FILE* fp = openTextFile((dir + "/databases/db_names.txt").c_str(), "w");
    closeTextFile(fp);

    remove((dir + "/databases/" + currentOpenDB + "/file_names.txt").c_str());
    // Delete created SSTs
    for (const string &name : sstNames) {
        remove((dir + "/databases/" + currentOpenDB + "/" + name + ".bin").c_str());
    }
    rmdir((dir + "/databases/" + currentOpenDB).c_str());

    sstNames.clear();
}
void testScanAcrossSSTs() {
    memtable->clearTree();
    assert(memtable->getRoot() == nullptr);
    assert(memtable->getTreeSize() == 0);

    currentOpenDB = "testScanAcrossSSTs";
    openDBPart1(currentOpenDB.c_str());

    int iteration = 1;
    uint64_t key;
    int64_t value;
    while (iteration < MEMTABLE_SIZE + 1) {
        key = static_cast<uint64_t>(iteration);
        value = static_cast<int64_t>(iteration);
        put(key, value);
        iteration++;
    }
    iteration = MEMTABLE_SIZE + 1;
    // Fill up data in another memtable that results in flush to disk
    while (iteration < (2 * MEMTABLE_SIZE) + 1) {
        key = static_cast<uint64_t>(iteration);
        value = static_cast<int64_t>(iteration);
        put(key, value);
        iteration++;
    }

    bool pageExists = true;
    int pageNum = 0;
    int64_t key_value_check;
    // Scan for result between two SSTs
    vector<KVPair> kvPairs = scan(MEMTABLE_SIZE - 3, MEMTABLE_SIZE + 3);
    sort(kvPairs.begin(), kvPairs.end());
    for(int i = 0; i < 7; ++i) {
        key_value_check = key = static_cast<int64_t>(MEMTABLE_SIZE - 3 + i);
        assert(key_value_check == kvPairs[i].getValue());
    }
     // clean up
    string dir = pwd();

    remove((dir + "/databases/db_names.txt").c_str());
    // Need to create db_names.txt again
    FILE* fp = openTextFile((dir + "/databases/db_names.txt").c_str(), "w");
    closeTextFile(fp);

    remove((dir + "/databases/" + currentOpenDB + "/file_names.txt").c_str());
    // Delete created SSTs
    for (const string &name : sstNames) {
        remove((dir + "/databases/" + currentOpenDB + "/" + name + ".bin").c_str());
    }
    rmdir((dir + "/databases/" + currentOpenDB).c_str());

    sstNames.clear();
}

void scanTestAll() {
    cout << "RUNNING SCAN API TESTS.." << endl << endl;

    cout << "Running SCAN API: Get scan result in memtable..." << endl;
    testScanInMemtable();
    cout << "testScanInMemtable passed" << endl;

    cout << "Running SCAN API: Get scan result in memtable with key2 out of range..." << endl;
    testScanInMemtableOutsideRange();
    cout << "testScanInMemtableOutsideRange passed" << endl;

    cout << "Running SCAN API: Get scan result in SST..." << endl;
    testScanInSST();
    cout << "testScanInSST passed" << endl;

    cout << "Running SCAN API: Get scan result across memtable and SST..." << endl;
    testScanAcrossMemtableAndSST();
    cout << "testScanAcrossMemtableAndSST passed" << endl;

    cout << "Running SCAN API: Get scan result across multiple SSTs..." << endl;
    testScanAcrossSSTs();
    cout << "testScanAcrossSSTs passed" << endl;

    cout << endl << "All SCAN API tests passed!" << endl << endl;
}