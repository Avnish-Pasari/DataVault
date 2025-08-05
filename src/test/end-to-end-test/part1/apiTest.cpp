#include "./apiTest.h"

void testEndToEndAPICalls() {
    memtable->clearTree();
    assert(memtable->getRoot() == nullptr);
    assert(memtable->getTreeSize() == 0);

    currentOpenDB = "testEndToEndAPICalls";
    openDBPart1(currentOpenDB.c_str());

    int iteration = 1;
    uint64_t key;
    int64_t value;
    
    cout << "PUT TEST: Write upto MEMTABLE_SIZE data..." << endl;
    while (iteration < MEMTABLE_SIZE + 5) {
        key = static_cast<uint64_t>(iteration);
        value = static_cast<int64_t>(iteration);
        put(key, value);
        iteration++;
    }
    cout << "Memtable is filled with valid data containing MEMTABLE_SIZE total entries!" << endl << endl;

    cout << "GET TEST: Get Value for Key 2..." << endl << endl;
    value = static_cast<int64_t>(2);
    assert(value == get(2));
    cout << "Value of Key 2 is verified successfully!" << endl << endl;

    cout << "SCAN TEST: Scan Result between key 1 and key 10..." << endl;
    vector<KVPair> scan_result = scan(1, 10);
    for (int i = 0; i < 10; ++i) {
        key = static_cast<uint64_t>(i + 1);
        value = static_cast<int64_t>(i + 1);
        assert(value == scan_result[i].getValue());
    }
    cout << "Scan results have been successfully verified!" << endl << endl;

    // Store a copy of sstNames and DB name since clearDB will do relevant clean up
    vector<string> sstNamesCopy = sstNames;
    string clearDBName = currentOpenDB;
    // When we flush to disk, the newly created SST will have the below name.
    // We need to delete this potentially created SST as well depending on whether the memtable has
    // any existing KV entries in it or not.
    string flushedSSTName = "SST_" + to_string(sstNames.size() + 1);

    closeDBPart1();
    assert(memtable->getRoot() == nullptr);
    assert(memtable->getTreeSize() == 0);
    assert(sstNames.size() == 0);

    cout << "Database closed successfully!" << endl;

    cout << "All API tests have passed!" << endl;
     // clean up
    string dir = pwd();

    remove((dir + "/databases/db_names.txt").c_str());
    // Need to create db_names.txt again
    FILE* fp = openTextFile((dir + "/databases/db_names.txt").c_str(), "w");
    closeTextFile(fp);

    remove((dir + "/databases/" + clearDBName + "/file_names.txt").c_str());
    // Delete created SSTs
    for (const string &name : sstNamesCopy) {
        remove((dir + "/databases/" + clearDBName + "/" + name + ".bin").c_str());
    }
    remove((dir + "/databases/" + clearDBName + "/" + flushedSSTName + ".bin").c_str());
    rmdir((dir + "/databases/" + clearDBName).c_str());
}

void testAllEndToEndAPICalls() {
    cout << "RUNNING END-TO-END API TESTS.." << endl << endl;
    testEndToEndAPICalls();
    cout << "testEndToEndAPICalls passed" << endl;
}