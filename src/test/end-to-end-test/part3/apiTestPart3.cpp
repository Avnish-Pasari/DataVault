#include "apiTestPart3.h"

void testEndToEndAPICallsPart3() {
    reset();

    openDBPart3("test-db-1");

    const int numKVPairs = 1000000;

    // perform insertions
    for (int i = 1; i <= numKVPairs; i++) {
        putPart3(i, i);
    }
    cout << "Insertions completed." << endl;

    // validate insertions using binary search
    for (int i = 1; i <= numKVPairs; i += 1000) {
        assert(get_binarySearch_part3(i) == i);
    }
    assert(get_binarySearch_part3(numKVPairs + 1) == INT64_MIN);
    assert(get_binarySearch_part3(123456) == 123456);
    cout << "Validated insertions using GET with binary search." << endl;

    // perform updates and validate scan functionality
    putPart3(123456, 654321);
    assert(get_binarySearch_part3(123456) == 654321);
    vector<KVPair> scanResults = scan_binarySearch_part3(100000, 150000);
    assert(scanResults.size() == 50001);
    for (int i = 0; i < 50001; i++) {
        if (i == 23456) {
            assert(scanResults[i].getKey() == 123456);
            assert(scanResults[i].getValue() == 654321);
        } else {
            assert(scanResults[i].getKey() == i + 100000);
            assert(scanResults[i].getValue() == i + 100000);
        }
    }
    cout << "Validated updates and scan functionality using binary search." << endl;

    // perform deletion
    deleteKey(150);
    assert(get_binarySearch_part3(150) == INT64_MIN);
    scanResults = scan_binarySearch_part3(100, 200);
    for (KVPair kv : scanResults) {
        assert(kv.getKey() != 150);
    }
    cout << "Validated deletion and GET using binary search." << endl;

    // close and reopen database
    closeDBPart3();
    openDBPart3("test-db-1");
    putPart3(123456, 123456);
    putPart3(150, 150);
    cout << "Closed and reopened database successfully." << endl;

    // validate insertions using b-tree
    for (int i = 1; i <= numKVPairs; i += 1000) {
        assert(get_bTree_part3(i) == i);
    }
    assert(get_bTree_part3(numKVPairs + 1) == INT64_MIN);
    assert(get_bTree_part3(123456) == 123456);
    cout << "Validated insertions using GET with b-tree." << endl;

    // perform updates and validate scan functionality
    putPart3(123456, 654321);
    assert(get_bTree_part3(123456) == 654321);
    scanResults = scan_bTree_part3(100000, 150000);
    assert(scanResults.size() == 50000);
    for (int i = 0; i < 50000; i++) {
        if (i == 23455) {
            assert(scanResults[i].getKey() == 123456);
            assert(scanResults[i].getValue() == 654321);
        } else {
            assert(scanResults[i].getKey() == i + 100001);
            assert(scanResults[i].getValue() == i + 100001);
        }
    }
    cout << "Validated updates and scan functionality using b-tree." << endl;

    // perform deletion
    deleteKey(150);
    assert(get_bTree_part3(150) == INT64_MIN);
    scanResults = scan_bTree_part3(100, 200);
    for (KVPair kv : scanResults) {
        assert(kv.getKey() != 150);
    }
    cout << "Validated deletion and GET using b-tree." << endl;

    cout << "testEndToEndAPICallsPart3 passed" << endl;
}