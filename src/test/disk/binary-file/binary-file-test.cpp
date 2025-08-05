#include "binary-file-test.h"

// Helper function that creates a KVPair vector of the given size
void helperCreateKVPairVector(int size, vector<KVPair> &kvPairs) {
    for(int i = 1; i <= size; i++) {
        KVPair kvPair(i, i * 3);
        kvPairs.push_back(kvPair);
    }
}

// Function to test writing to a binary file
void testWriteToBinaryFile() {
    // Fill the SST up completely
    // Test the openBinaryFile function
    FILE *fp = openBinaryFile("SST-CompletelyFull.dat", "wb");
    // Test the writeSingleKVPairToBinaryFile function
    for(int i = 1; i <= MEMTABLE_SIZE; i++) {
        KVPair kvPair(i, i * 3);
        writeSingleKVPairToBinaryFile(fp, kvPair);
    }
    // Test the closeBinaryFile function
    closeBinaryFile(fp);


    // Fill the SST partially (2 full pages less)
    // Test the openBinaryFile function
    fp = openBinaryFile("SST-FullMinus2Pages.dat", "wb");
    // Test the writeSingleKVPairToBinaryFile function
    for(int i = 1; i <= MEMTABLE_SIZE - (2 * KV_PAIRS_PER_PAGE); i++) {
        KVPair kvPair(i, i * 3);
        writeSingleKVPairToBinaryFile(fp, kvPair);
    }
    // Test the closeBinaryFile function
    closeBinaryFile(fp);


    // First page of SST is partially filled
    vector<KVPair> firstPagePartiallyFilled;
    helperCreateKVPairVector(KV_PAIRS_PER_PAGE - 55, firstPagePartiallyFilled);
    writeKVPairVectorToBinaryFile("SST-FirstPagePartiallyFilled.dat", firstPagePartiallyFilled);

    // Last page of SST is partially filled
    vector<KVPair> lastPagePartiallyFilled;
    helperCreateKVPairVector(MEMTABLE_SIZE - (KV_PAIRS_PER_PAGE / 2), lastPagePartiallyFilled);
    writeKVPairVectorToBinaryFile("SST-LastPagePartiallyFilled.dat", lastPagePartiallyFilled);

    // Some middle page of SST is partially filled
    vector<KVPair> middlePagePartiallyFilled;
    helperCreateKVPairVector(MEMTABLE_SIZE - (KV_PAIRS_PER_PAGE * 5) + (KV_PAIRS_PER_PAGE / 2), middlePagePartiallyFilled);
    writeKVPairVectorToBinaryFile("SST-MiddlePagePartiallyFilled.dat", middlePagePartiallyFilled);
}

void testReadFromBinaryFile() {

    KVPair kvPairs[KV_PAIRS_PER_PAGE];

    // Read the SST-CompletelyFull.dat file
    FILE *fp = openBinaryFile("SST-CompletelyFull.dat", "rb");
    for(int i = 0; i < PAGES_PER_SST; i++) {
        int size = readPageFromBinaryFile(fp, i, kvPairs);
        if(size != KV_PAIRS_PER_PAGE) {
            cout << "[TEST] Error: reading page " << i << " from SST-CompletelyFull.dat" << endl;
            exit(1);
        }
    }
    closeBinaryFile(fp);

    // Read the SST-FullMinus2Pages.dat file
    fp = openBinaryFile("SST-FullMinus2Pages.dat", "rb");
    for(int i = 0; i < PAGES_PER_SST; i++) {
        int size = readPageFromBinaryFile(fp, i, kvPairs);
        if(i < PAGES_PER_SST - 2) {
            if(size != KV_PAIRS_PER_PAGE) {
                cout << "[TEST] Error: reading page " << i << " from SST-FullMinus2Pages.dat" << endl;
                exit(1);
            }
        } else {
            if(size != 0) {
                cout << "[TEST] Error: reading page " << i << " from SST-FullMinus2Pages.dat" << endl;
                exit(1);
            }
        }
    }
    closeBinaryFile(fp);

    // Read the SST-FirstPagePartiallyFilled.dat file
    fp = openBinaryFile("SST-FirstPagePartiallyFilled.dat", "rb");
    for(int i = 0; i < PAGES_PER_SST; i++) {
        int size = readPageFromBinaryFile(fp, i, kvPairs);
        if(i == 0) {
            if(size != KV_PAIRS_PER_PAGE - 55) {
                cout << "[TEST] Error: reading page " << i << " from SST-FirstPagePartiallyFilled.dat" << endl;
                exit(1);
            }
        } else {
            if(size != 0) {
                cout << "[TEST] Error: reading page " << i << " from SST-FirstPagePartiallyFilled.dat" << endl;
                exit(1);
            }
        }
    }
    closeBinaryFile(fp);

    // Read the SST-LastPagePartiallyFilled.dat file
    fp = openBinaryFile("SST-LastPagePartiallyFilled.dat", "rb");
    for(int i = 0; i < PAGES_PER_SST; i++) {
        int size = readPageFromBinaryFile(fp, i, kvPairs);
        if(i == PAGES_PER_SST - 1) {
            if(size != KV_PAIRS_PER_PAGE / 2) {
                cout << "[TEST] Error: reading page " << i << " from SST-LastPagePartiallyFilled.dat" << endl;
                exit(1);
            }
        } else {
            if(size != KV_PAIRS_PER_PAGE) {
                cout << "[TEST] Error: reading page " << i << " from SST-LastPagePartiallyFilled.dat" << endl;
                exit(1);
            }
        }
    }
    closeBinaryFile(fp);

    // Read the SST-MiddlePagePartiallyFilled.dat file
    fp = openBinaryFile("SST-MiddlePagePartiallyFilled.dat", "rb");
    for(int i = 0; i < PAGES_PER_SST; i++) {
        int size = readPageFromBinaryFile(fp, i, kvPairs);
        if(i == PAGES_PER_SST - 5) {
            if(size != KV_PAIRS_PER_PAGE / 2) {
                cout << "[TEST] Error: reading page " << i << " from SST-MiddlePagePartiallyFilled.dat" << endl;
                exit(1);
            }
        } else if (i < PAGES_PER_SST - 5) {
            if(size != KV_PAIRS_PER_PAGE) {
                cout << "[TEST] Error: reading page " << i << " from SST-MiddlePagePartiallyFilled.dat" << endl;
                exit(1);
            }
        } else {
            if(size != 0) {
                cout << "[TEST] Error: reading page " << i << " from SST-MiddlePagePartiallyFilled.dat" << endl;
                exit(1);
            }
        }
    }
    closeBinaryFile(fp);

}

void testWriteReadCharPageToBinaryFile() {
    char arr[4096];
    for(int i = 0; i < 4096; i++) {
        arr[i] = i;
    }
    writeCharArrayToBinaryFile("SST-CharTest.dat", arr, 4096);

    char arr2[4096];
    FILE *fp = openBinaryFile("SST-CharTest.dat", "rb");
    int size = readCharPageFromBinaryFile(fp, 0, arr2);
    if(size != 4096) {
        cout << "[TEST] Error: reading char page from SST-CharTest.dat" << endl;
        exit(1);
    }
    for(int i = 0; i < 4096; i++) {
        if(arr[i] != arr2[i]) {
            cout << "[TEST] Error: reading char page from SST-CharTest.dat" << endl;
            exit(1);
        }
    }
    closeBinaryFile(fp);

    remove("SST-CharTest.dat");
}

void testWriteReadCharMultipleCharToBinaryFile() {
    // Fill the Char pages partially
    char arr[5000];
    for(int i = 0; i < 5000; i++) {
        arr[i] = i;
    }
    writeCharArrayToBinaryFile("SST-CharTest.dat", arr, 5000);

    char arr2[4096];
    FILE *fp = openBinaryFile("SST-CharTest.dat", "rb");
    int size = readCharPageFromBinaryFile(fp, 0, arr2);
    if(size != 4096) {
        cout << "[TEST] Error: reading char page from SST-CharTest.dat" << endl;
        exit(1);
    }
    for(int i = 0; i < 4096; i++) {
        if(arr[i] != arr2[i]) {
            cout << "[TEST] Error: reading char page from SST-CharTest.dat" << endl;
            exit(1);
        }
    }

    size = readCharPageFromBinaryFile(fp, 1, arr2);
    if(size != 904) {
        cout << "[TEST] Error: reading char page from SST-CharTest.dat" << endl;
        exit(1);
    }
    for(int i = 0; i < 904; i++) {
        if(arr[i + 4096] != arr2[i]) {
            cout << "[TEST] Error: reading char page from SST-CharTest.dat" << endl;
            exit(1);
        }
    }

    size = readCharPageFromBinaryFile(fp, 2, arr2);
    if(size != 0) {
        cout << "[TEST] Error: reading char page from SST-CharTest.dat" << endl;
        exit(1);
    }

    closeBinaryFile(fp);

    remove("SST-CharTest.dat");

}

void binaryFileTestAll() {

    cout << "RUNNING BINARY FILE TESTS..." << endl << endl;
    testWriteToBinaryFile();
    testReadFromBinaryFile();
    cout << "[TEST] Success: Open Binary File" << endl;
    cout << "[TEST] Success: Write Single KV Pair to Binary File" << endl;
    cout << "[TEST] Success: Read Page from Binary File" << endl;
    cout << "[TEST] Success: Write KV Pair Vector to Binary File" << endl;
    cout << "[TEST] Success: Close Binary File" << endl;

    void testWriteReadCharPageToBinaryFile();
    void testWriteReadCharMultipleCharToBinaryFile();
    cout << "[TEST] Success: Write Char Array to Binary File" << endl;
    cout << "[TEST] Success: Read Char Page from Binary File" << endl;

    cout << "FINISHED RUNNING BINARY FILE TESTS..." << endl << endl;

    // Clean up
    remove("SST-CompletelyFull.dat");
    remove("SST-FullMinus2Pages.dat");
    remove("SST-FirstPagePartiallyFilled.dat");
    remove("SST-LastPagePartiallyFilled.dat");
    remove("SST-MiddlePagePartiallyFilled.dat");
}