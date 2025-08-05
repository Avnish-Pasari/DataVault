#include "../put/putTest.h"

void testPutFlushToBTreeSST() {
    memtable->clearTree();
    assert(memtable->getRoot() == nullptr);
    assert(memtable->getTreeSize() == 0);
    
    currentOpenDB = "testPutPart2FlushToSST";
    openDBPart2(currentOpenDB.c_str());

    vector<KVPair> allPairs;
    int size = 0;
    // Starting the loop from 1 since key's cannot take value 0 in our implementation
    for (int64_t i = 1; i < MEMTABLE_SIZE + 2; ++i) {
        uint64_t key_put = static_cast<uint64_t>(i);
        int64_t value_put = static_cast<int64_t>(i);
        putPart2(key_put, value_put);
        allPairs.push_back(KVPair(key_put, value_put));
        size++;
    }
    bool pageExists = true;
    int pageNum = 1;    // Because the file will have 1 internal node
    int64_t key_value_check;
    

    for (const string &name : sstNames) {
        string filePath = pwd() + "/databases/" + currentOpenDB + "/" + name + ".bin";
        FILE* fp = openBinaryFile(filePath.c_str(), "rb");
        int allPairsIdx = 0;

        for (int64_t i = 1; i < MEMTABLE_SIZE + 2; i++) {
            int64_t foundValue = get_binarySearch(allPairs[i].getKey());
            assert(foundValue == allPairs[i].getValue());
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


void putPart2TestAll() {
    cout << "RUNNING PUT API TESTS.." << endl << endl;

    cout << "Running PUT API: Flush KV pairs to a B-Tree SST..." << endl;
    testPutFlushToBTreeSST();
    cout << "testPutFlushToBTreeSST passed" << endl;

    cout << endl << "All PUT API tests passed!" << endl << endl;
}