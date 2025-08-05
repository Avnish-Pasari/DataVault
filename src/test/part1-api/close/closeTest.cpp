#include "closeTest.h"

void testCloseDBWithHalfFilledMemtable() {
    memtable->clearTree();
    assert(memtable->getRoot() == nullptr);
    assert(memtable->getTreeSize() == 0);

    currentOpenDB = "testCloseDB";
    openDBPart1(currentOpenDB.c_str());

    int iteration = 1;
    uint64_t key;
    int64_t value;
    while (iteration < MEMTABLE_SIZE / 2) {
        key = static_cast<uint64_t>(iteration);
        value = static_cast<int64_t>(iteration);
        put(key, value);
        iteration++;
    }

    closeDBPart1();
    assert(memtable->getRoot() == nullptr);
    assert(memtable->getTreeSize() == 0);
    // Reopen the previously closed database and check if values in the SST persist
    currentOpenDB = "testCloseDB";
    openDBPart1(currentOpenDB.c_str());

    for (const string &name : sstNames) {
        string filePath = pwd() + "/databases/" + currentOpenDB + "/" + name + ".bin";
        FILE* fp = openBinaryFile(filePath.c_str(), "rb");

        int pageNumber = 0;
        long fileSize = getFileSize(fp);
        long num_pages = ceil(static_cast<double>(fileSize) / PAGE_SIZE);
        int size = 0;
        int itr_check = 1;
        KVPair kvPairs[KV_PAIRS_PER_PAGE];
        while (pageNumber < num_pages) {
            size = readPageFromBinaryFile(fp, pageNumber, kvPairs);
            if (size == 0) {
                break;
            }
            for (int i = 0; i < size; ++i) {
                uint64_t key = static_cast<uint64_t>(itr_check);
                int64_t key_value_check = static_cast<int64_t>(itr_check);
                assert(key_value_check == kvPairs[i].getValue());
                itr_check++;
            }
            pageNumber++;
        }
        closeBinaryFile(fp);
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
void testCloseEmptyDB() {
    memtable->clearTree();
    assert(memtable->getRoot() == nullptr);
    assert(memtable->getTreeSize() == 0);

    currentOpenDB = "testCloseEmptyDB";
    openDBPart1(currentOpenDB.c_str());
    closeDBPart1();
    assert(memtable->getRoot() == nullptr);
    assert(memtable->getTreeSize() == 0);

    // Reopen the DB and assert the loaded SST names vector is empty indicating no SSTs were created for this DB.
    currentOpenDB = "testCloseEmptyDB";
    openDBPart1(currentOpenDB.c_str());
    assert(sstNames.size() == 0);

    // clean up
    string dir = pwd();

    remove((dir + "/databases/db_names.txt").c_str());
    // Need to create db_names.txt again
    FILE* fp = openTextFile((dir + "/databases/db_names.txt").c_str(), "w");
    closeTextFile(fp);
    remove((dir + "/databases/" + currentOpenDB + "/file_names.txt").c_str());
    rmdir((dir + "/databases/" + currentOpenDB).c_str());

    sstNames.clear();
}

void closeTestAll() {
    cout << "RUNNING CLOSE DB API TESTS.." << endl << endl;

    cout << "Running CLOSE DB API: Close DB with Half Filled Memtable..." << endl;
    testCloseDBWithHalfFilledMemtable();
    cout << "testCloseDBWithHalfFilledMemtable passed" << endl;

    cout << "Running CLOSE DB API: Close Empty DB..." << endl;
    testCloseEmptyDB();
    cout << "testCloseEmptyDB passed" << endl;

    cout << endl << "All Close DB API tests passed!" << endl << endl;
}