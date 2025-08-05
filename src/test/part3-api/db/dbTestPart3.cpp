#include "./dbTestPart3.h"

void testOpenDBPart3() {
    reset();

    int res = openDBPart3("test-db-0");

    // do checks on freshly opened db
    assert(res == 1);
    assert(lsmNames.size() == 1);
    assert(lsmNames.at(0) == "");
    assert(currentOpenDB == "test-db-0");
    assert(isNameInTextFile("../databases/db_names.txt", "test-db-0"));
    assert(system("test -d ../databases/test-db-0") == 0);
    assert(system("test -f ../databases/test-db-0/file_names.txt") == 0);
    
    std::ifstream file("../databases/test-db-0/file_names.txt", std::ios::binary | std::ios::ate);
    assert(file.tellg() == 0);

    // close db 
    closeDBPart3();
    assert(lsmNames.size() == 0);
    assert(currentOpenDB == "");
    assert(isNameInTextFile("../databases/db_names.txt", "test-db-0"));

    // reopen db
    res = openDBPart3("test-db-0");
    assert(res == 2);
    assert(lsmNames.size() == 1);
    assert(lsmNames.at(0) == "");
    assert(currentOpenDB == "test-db-0");
    assert(isNameInTextFile("../databases/db_names.txt", "test-db-0"));
    assert(system("test -d ../databases/test-db-0") == 0);
    assert(system("test -f ../databases/test-db-0/file_names.txt") == 0);

    // close db
    closeDBPart3();
    
    // reopen db and do a bunch of puts
    res = openDBPart3("test-db-0");
    assert(res == 2);
    for (int i = 1; i <= 100; i++) {
        putPart3(i, i);
    }
    closeDBPart3();

    std::ifstream file2("../databases/test-db-0/file_names.txt", std::ios::binary | std::ios::ate);
    assert(file2.tellg() != 0);

    // reopen db
    res = openDBPart3("test-db-0");
    assert(res == 2);

    // check that the flush to disk worked
    assert(lsmNames.size() == 2);
    assert(lsmNames.at(0) == "");
    assert(lsmNames.at(1) != "");
}

void testCloseDBHalfFilledMemtable() {
    reset();
    
    // open db and add 100 kv pairs
    int res = openDBPart3("test-db-1");
    for (int i = 1; i <= 100; i++) {
        putPart3(i, i);
    }
    closeDBPart3();

    // assert that BF, BT, SST all created
    assert(system("test -f ../databases/test-db-1/BF2.bin") == 0);
    assert(system("test -f ../databases/test-db-1/BT2.bin") == 0);
    assert(system("test -f ../databases/test-db-1/SST2.bin") == 0);

    // reopen db and do sanity checks
    res = openDBPart3("test-db-1");
    assert(res == 2);
    assert(lsmNames.size() == 2);
    assert(lsmNames.at(0) == "");
    assert(lsmNames.at(1) != "");

    // check values
    for (int i = 1; i <= 100; i++) {
        assert(get_binarySearch_part3(i) == i);
        assert(get_bTree_part3(i) == i);
    }
    assert(get_binarySearch_part3(101) == INT64_MIN);
    assert(get_bTree_part3(101) == INT64_MIN);

    // // validate SST file number of kv pairs and sorted nature
    validateSSTFile("../databases/test-db-1/SST2.bin", 100);
}

void testCloseDBForceCompaction() {
    reset();

    // open db and add enough kv pairs to force compaction
    int res = openDBPart3("test-db-2");
    for (int i = 1; i <= MEMTABLE_SIZE * 2 + 1; i++) {
        putPart3(i, i);
    }
    closeDBPart3();

    // assert there are 2 levels, level 2 has SST level 1 has 1 with 1 key
    assert(system("test -f ../databases/test-db-1/BF4.bin") != 0);
    assert(system("test -f ../databases/test-db-1/BT4.bin") != 0);
    assert(system("test -f ../databases/test-db-1/BF6.bin") != 0);
    assert(system("test -f ../databases/test-db-1/BT6.bin") != 0);
    assert(system("test -f ../databases/test-db-1/SST6.bin") != 0);
    assert(system("test -f ../databases/test-db-1/SST4.bin") != 0);

    // // reopen db and do sanity checks
    res = openDBPart3("test-db-2");
    assert(res == 2);
    assert(lsmNames.size() == 3);
    assert(lsmNames.at(0) == "");
    assert(lsmNames.at(1) != "");
    assert(lsmNames.at(2) != "");

    // check values
    for (int i = 1; i <= MEMTABLE_SIZE * 2 + 1; i += 1000) {
        assert(get_binarySearch_part3(i) == i);
        assert(get_bTree_part3(i) == i);
    }
    assert(get_binarySearch_part3(MEMTABLE_SIZE * 2 + 2) == INT64_MIN);
    assert(get_bTree_part3(MEMTABLE_SIZE * 2 + 2) == INT64_MIN);

    // validate SST file number of kv pairs and sorted nature
    validateSSTFile("../databases/test-db-2/SST4.bin", MEMTABLE_SIZE * 2);
} 

void validateSSTFile(string sstName, int expectedNumKVPairs) {
    FILE *fp = openBinaryFile(sstName.c_str(), "rb");

    KVPair page[KV_PAIRS_PER_PAGE];
    vector<KVPair> kvPairs;

    // ensure expected number of KV Pairs
    int numRead = 0;
    int pageIndex = 0;
    while (true) {
        int elementsRead = readPageFromBinaryFile(fp, pageIndex++, page);
        if (elementsRead == 0) {
            break;
        }
        kvPairs.insert(kvPairs.end(), page, page + elementsRead);
        numRead += elementsRead;
    }

    assert(numRead == expectedNumKVPairs);

    // confirm that the KV Pairs are sorted
    for (int i = 1; i < kvPairs.size(); i++) {
        assert(kvPairs[i - 1].getKey() < kvPairs[i].getKey());
    }

    closeBinaryFile(fp);
}

void dbPart3TestAll() {
    cout << endl << "Running Part 3 API DB Tests..." << endl;
    
    cout << "Running testOpenDBPart3..." << endl;
    testOpenDBPart3();
    cout << "testOpenDBPart3 passed" << endl;
    
    cout << "Running testCloseDBHalfFilledMemtable..." << endl;
    testCloseDBHalfFilledMemtable();
    cout << "testCloseDBHalfFilledMemtable passed" << endl;
    cout << "Running testCloseDBForceCompaction..." << endl;
    testCloseDBForceCompaction();
    cout << "testCloseDBForceCompaction passed" << endl;

    cout << "All Part 3 API DB Tests passed!" << endl << endl;
}