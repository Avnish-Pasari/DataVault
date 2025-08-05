#include <iostream>
#include "./part1-api/db/db-part1.h"
#include "./avl/avl.h"
#include "./part1-api/get/get.h"
#include "./part1-api/put/put.h"
#include "./part1-api/scan/scan.h"
#include "../kv-store/util/namespace-std.h"
#include "../kv-store/part1-api/db/db-part1.h"
#include "../kv-store/part1-api/get/get.h"
#include "../kv-store/part1-api/put/put.h"
#include "../kv-store/part1-api/scan/scan.h"
#include "../kv-store/part2-api/db/db-part2.h"
#include "../kv-store/part2-api/get/get-binarySearch/get-binarySearch.h"
#include "../kv-store/part2-api/get/get-bTree/get-bTree.h"
#include "../kv-store/part2-api/put/put-part2.h"
#include "../kv-store/part2-api/scan/scan-binarySearch/scan-binarySearch.h"
#include "../kv-store/part2-api/scan/scan-bTree/scan-bTree.h"
#include "../kv-store/part3-api/db/db-part3.h"
#include "../kv-store/part3-api/get/get-binarySearch/get-binarySearch-part3.h"
#include "../kv-store/part3-api/get/get-bTree/get-bTree-part3.h"
#include "../kv-store/part3-api/put/put-part3.h"
#include "../kv-store/part3-api/scan/scan-binarySearch/scan-binarySearch-part3.h"
#include "../kv-store/part3-api/scan/scan-bTree/scan-bTree-part3.h"

// Create a new memtable
AVLTree* memtable =  new AVLTree();
// Keep track of current openDB
string currentOpenDB = "";
// Keep track of SSTs names flushed to disk
vector<string> sstNames;
// Stores the buffer poll
BufferPool* bufferPool = new BufferPool();

// Flag to enable/disable buffer pool
bool BUFFER_POOL_ENABLED_FLAG = true;
// Flag to enable/disable bloom filters
bool BLOOM_FILTER_ENABLED_FLAG = true;

// Keep track of LSMs names and levels flushed to disk
vector<string> lsmNames(1, "");
// Keep track of SST name counter
int sstNameCounter = 1;

// Just need a main.cpp file for compilation purposes
int main() {
    // custom code here
    return 0;
}