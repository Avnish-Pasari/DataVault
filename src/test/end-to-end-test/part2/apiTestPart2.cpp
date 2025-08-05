#include "apiTestPart2.h"

void testEndToEndAPICallsPart2() {
    reset();

    openDBPart2("test-db-1");

    const int numKVPairs = 100000;

    // perform insertions
    for (int i = 1; i <= numKVPairs; i++) {
        putPart2(i, i);
    }
    cout << "Insertions completed." << endl;

    // validate insertions using binary search
    for (int i = 1; i <= numKVPairs; i += 1000) {
        assert(get_binarySearch(i) == i);
    }
    assert(get_binarySearch(numKVPairs + 1) == INT64_MIN);
    assert(get_binarySearch(12345) == 12345);
    cout << "Validated insertions using GET with binary search." << endl;

    // validate scan functionality
    vector<KVPair> scanResults = scan_binarySearch(10000, 15000);
    assert(scanResults.size() == 5001);
    for (int i = 0; i < 5001; i++) {
        assert(scanResults[i].getKey() == i + 10000);
        assert(scanResults[i].getValue() == i + 10000);
    }
    cout << "Validated scan functionality using binary search." << endl;

    // close and reopen database
    closeDBPart2();
    openDBPart2("test-db-1");
    cout << "Closed and reopened database successfully." << endl;

    // validate insertions using b-tree
    for (int i = 1; i <= numKVPairs; i += 1000) {
        assert(get_bTree(i) == i);
    }
    assert(get_bTree(numKVPairs + 1) == INT64_MIN);
    assert(get_bTree(12345) == 12345);
    cout << "Validated insertions using GET with b-tree." << endl;

    // validate scan functionality
    scanResults = scan_bTree(10000, 15000);
    assert(scanResults.size() == 5001);
    for (int i = 0; i < 5001; i++) {
        assert(scanResults[i].getKey() == i + 10000);
        assert(scanResults[i].getValue() == i + 10000);
    }
    cout << "Validated scan functionality using b-tree." << endl;

    cout << "testEndToEndAPICallsPart2 passed" << endl;
}   