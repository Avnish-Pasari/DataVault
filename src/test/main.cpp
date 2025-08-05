#include <iostream>
#include "../kv-store/util/namespace-std.h"

#include "../test/avl/avlTest.h"
#include "../test/disk/binary-file/binary-file-test.h"
#include "../test/part1-api//db/dbTest.h"
#include "../test/disk/text-file/text-file-test.h"
#include "../test/part1-api/get/getTest.h"
#include "../test/part1-api/put/putTest.h"
#include "../test/part1-api/scan/scanTest.h"
#include "../test/part1-api/close/closeTest.h"
#include "../test/end-to-end-test/part1/apiTest.h"
#include "../test/deque/dequeTest.h"
#include "../test/buffer-pool/bufferPoolTest.h"
#include "../test/b-tree/bTreeTest.h"
#include "../test/bloom-filters/bloomFilterTest.h"
#include "../test/part2-api/get/get-BTree/getB-TreeTest.h"
#include "../test/part2-api/get/get-binarySearch/get-binarySearchTest.h"
#include "../test/part2-api/put/putTest.h"
#include "../test/part2-api/scan/scan-binarySearch/scan-binarySearchTest.h"
#include "../test/part2-api/scan/scan-bTree/scan-bTreeTest.h"
#include "../test/end-to-end-test/part2/apiTestPart2.h"
#include "../test/part3-api/db/dbTestPart3.h"
#include "../test/part3-api/put/putTestPart3.h"
#include "../test/part3-api/delete/deleteTestPart3.h"
#include "../test/part3-api/get/get-binarySearch/get-binarySearchTest-part3.h"
#include "../test/part3-api/get/get-bTree/get-bTreeTest-part3.h"
#include "../test/part3-api/scan/scan-binarySearch/scan-binarySearchTest-part3.h"
#include "../test/part3-api/scan/scan-bTree/scan-bTreeTest-part3.h"
#include "../test/end-to-end-test/part3/apiTestPart3.h"

AVLTree* memtable = new AVLTree();
string currentOpenDB = "";
vector<string> sstNames;
BufferPool* bufferPool = new BufferPool();
vector<string> lsmNames(1, "");
int sstNameCounter = 1;

bool BUFFER_POOL_ENABLED_FLAG = true;
bool BLOOM_FILTER_ENABLED_FLAG = true;

int main() {
    cout << "RUNNING UNIT TESTS..." << endl << endl;

    // Tests related to Part 1
    cout << endl << "RUNNING AVL UNIT TESTS..." << endl;
    avlTestAll();
    cout << endl << "RUNNING DISK UNIT TESTS..." << endl;
    binaryFileTestAll();
    textFileTestAll();
    cout << endl << "RUNNING TESTS FOR PART 1..." << endl << endl;
    getTestAll();
    putTestAll();
    scanTestAll();
    dbTestAll(); // tests openDB
    closeTestAll(); // tests closeDB
    cout << "RUNNING END-TO-END TESTS FOR PART 1..." << endl;
    testAllEndToEndAPICalls();
    cout << endl << "End-to-end tests for part 1 passed!" << endl << endl; 


    // Tests related to Part 2
    cout << endl << "RUNNING BUFFER POOL TESTS..." << endl;
    dequeTestAll();
    bufferPoolTestAll();
    cout << endl << "RUNNING B-TREE TESTS..." << endl;
    bTreeTestAll();
    cout << endl << "RUNNING TESTS FOR PART 2..." << endl << endl;
    putPart2TestAll();
    getPart2BinarySearchTestAll();
    getBTreeTestAll();
    scanPart2BinarySearchTestAll();
    scanPart2BTreeTestAll();
    cout << endl << "RUNNING END-TO-END TEST FOR PART 2..." << endl << endl;
    testEndToEndAPICallsPart2();
    cout << endl << "End-to-end tests for part 2 passed!" << endl << endl;


    // Tests related to Part 3
    cout << endl << "RUNNING BLOOM FILTER TESTS..." << endl;
    bloomFilterTestAll(); 
    cout << endl << "RUNNING TESTS FOR PART 3..." << endl << endl;
    dbPart3TestAll();
    putPart3TestAll();
    deletePart3TestAll();
    getPart3BinarySearchTestAll();
    getPart3BTreeTestAll();
    scanPart3BinarySearchTestAll();
    scanPart3BTreeTestAll();
    cout << endl << "RUNNING END-TO-END TEST FOR PART 3..." << endl << endl;
    testEndToEndAPICallsPart3();
    cout << endl << "End-to-end tests for part 3 passed!" << endl << endl;

    // clean up
    system("./clearDbs.sh");

    cout << "" << endl;
    cout << "ALL TESTS HAVE PASSED!" << endl;
    cout << "" << endl;
}