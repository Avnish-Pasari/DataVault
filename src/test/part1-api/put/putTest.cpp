#include "../put/putTest.h"

void testPutKVInMemtable() {
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
    while (iteration < 10) {
        key = static_cast<uint64_t>(iteration);
        key_value_check = static_cast<int64_t>(iteration);
        assert(key_value_check == get(key));
        iteration += 1;
    }
}

void testPutFlushToSST() {
    memtable->clearTree();
    assert(memtable->getRoot() == nullptr);
    assert(memtable->getTreeSize() == 0);
    
    currentOpenDB = "testPutFlushToSST";
    openDBPart1(currentOpenDB.c_str());

    int iteration = 1;
    uint64_t key;
    int64_t value;
    while (iteration < MEMTABLE_SIZE + 2) {
        key = static_cast<uint64_t>(iteration);
        value = static_cast<int64_t>(iteration);
        put(key, value);
        iteration++;
    }

    bool pageExists = true;
    int pageNum = 0;
    int64_t key_value_check;
    
    for (const string &name : sstNames) {
        string filePath = pwd() + "/databases/" + currentOpenDB + "/" + name + ".bin";
        FILE* fp = openBinaryFile(filePath.c_str(), "rb");
        while(pageExists) {
            KVPair kvPairs[KV_PAIRS_PER_PAGE];
            int size = readPageFromBinaryFile(fp, pageNum, kvPairs);
            iteration = 1;
            if (size > 0) {
                while (iteration < size) {
                    key = kvPairs[iteration].getKey();
                    key_value_check = kvPairs[iteration].getValue();
                    assert(key_value_check == get(key));
                    iteration ++;
                }
                pageNum++;
            }
            else {
                pageExists = false;
                break;
            }
        }
        closeBinaryFile(fp);
    }
    // clean up
    string dir = pwd();

    remove((dir + "/databases/db_names.txt").c_str());
    // Need to create db_names.txt again
    FILE* closeFP = openTextFile((dir + "/databases/db_names.txt").c_str(), "w");
    closeTextFile(closeFP);

    remove((dir + "/databases/" + currentOpenDB + "/file_names.txt").c_str());
    // Delete created SSTs
    for (const string &name : sstNames) {
        remove((dir + "/databases/" + currentOpenDB + "/" + name + ".bin").c_str());
    }
    rmdir((dir + "/databases/" + currentOpenDB).c_str());

    sstNames.clear();
}

void putTestAll() {
    cout << "RUNNING PUT API TESTS.." << endl << endl;

    cout << "Running PUT API: PUT KV pairs in memtable..." << endl;
    testPutKVInMemtable();
    cout << "testPutKVInMemtable passed" << endl;

    cout << "Running PUT API: Flush KV pairs to SST..." << endl;
    testPutFlushToSST();
    cout << "testPutFlushToSST passed" << endl;

    cout << endl << "All PUT API tests passed!" << endl << endl;
}