#include "../../kv-store/util/namespace-std.h"
#include "../../kv-store/util/global.h"
#include "../../kv-store/util/common.h"
#include "../../kv-store/disk/text-file/text-file.h"
#include "../../kv-store/part2-api/db/db-part2.h"
#include "../../kv-store/part2-api/get/get-binarySearch/get-binarySearch.h"
#include "../../kv-store/part2-api/get/get-bTree/get-bTree.h"
#include "../../kv-store/part2-api/put/put-part2.h"

#include <random>
#include <unordered_set>
#include <chrono>
#include <cstdlib>
#include <iostream>

AVLTree* memtable = new AVLTree();
string currentOpenDB = "";
vector<string> sstNames;
BufferPool* bufferPool = new BufferPool();
vector<string> lsmNames(1, "");
int sstNameCounter = 1;

bool BUFFER_POOL_ENABLED_FLAG = true;
bool BLOOM_FILTER_ENABLED_FLAG = true;

random_device rd;
// Use Mersenne Twister as the  Pseudo-Random Number Generator (PRNG)
// Seed with 1
mt19937 gen(1);

void addData(int mb) {
    int numKVPairsToInsert = mb * 65536; // 1MB = 1024KB = 1024 * 1024 Bytes = 1024 * (1024 / 16) = 65536 KV pairs
    
    uniform_int_distribution<uint64_t> keyDis(1, UINT64_MAX);

    for (int i = 0; i < numKVPairsToInsert; i++) {
        uint64_t key = keyDis(gen);
        putPart2(key, 1);
    }
}

void generateUniformlyRandomKeys(int numKeys, uint64_t *keys) {
    
    uniform_int_distribution<uint64_t> keyDis(1, UINT64_MAX);

    for (int i = 0; i < numKeys; i++) {
        keys[i] = keyDis(gen);
    }
}

void part2() {

    FILE *outfileBufferPool = openTextFile("./experiments/part2/part2-bufferpool.txt", "w");

    int oldNumMb = 0;
    for (int i = 0; i <= 10; i++) { 

        const int numKeysToSearch = 500;
        uint64_t *keys = new uint64_t[numKeysToSearch];
        generateUniformlyRandomKeys(numKeysToSearch, keys);

        cout << " " << endl; // Add a new line for better readability in console

        int numMb = 1 << i;

        cout << "Increasing data size to be: " << numMb << " MB" << endl;
        addData(numMb - oldNumMb);
        oldNumMb = numMb;
        cout << "Finished increasing data size to be: " << numMb << " MB" << endl;

        // Run experiment for buffer pool (enabled) & binary search
        cout << "Running binary search GET operations on data size: " << numMb << " MB" << endl;

        auto start = chrono::high_resolution_clock::now();
        for (int j = 0; j < numKeysToSearch; j++) {
            get_binarySearch(keys[j]);
        }
        auto stop = chrono::high_resolution_clock::now();

        double elapsedBufferPoolBinarySearch = chrono::duration_cast<chrono::duration<double> >(stop - start).count();
        double throughputBufferPoolBinarySearch = static_cast<double>(numKeysToSearch) / elapsedBufferPoolBinarySearch;
        
        cout << "Finished running binary search GET operations on data size: " << numMb << " MB" << endl;

        // Run experiment for buffer pool (enabled) & b-tree
        cout << "Running B-Tree GET operations on data size: " << numMb << " MB" << endl;

        start = chrono::high_resolution_clock::now();
        for (int j = 0; j < numKeysToSearch; j++) {
            get_bTree(keys[j]);
        }
        stop = chrono::high_resolution_clock::now();

        double elapsedBufferPoolBTree = chrono::duration_cast<chrono::duration<double> >(stop - start).count();
        double throughputBufferPoolBTree = static_cast<double>(numKeysToSearch) / elapsedBufferPoolBTree;
        
        cout << "Finished running B-Tree GET operations on data size: " << numMb << " MB" << endl;

        fprintf(outfileBufferPool, "data_size=%d, throughput_binarysearch=%.2f, throughput_btree=%.2f\n",
                numMb, throughputBufferPoolBinarySearch, throughputBufferPoolBTree);

        delete[] keys;
    }

    closeTextFile(outfileBufferPool);

}

int main() {

    auto start_exp = chrono::high_resolution_clock::now();

    cout << "Starting to run part 2 experiments..." << endl;

    // Clear the system
    system("./clearDbs.sh");

    // open database
    string dbName = "part2-experiments-database";
    openDBPart2(dbName.c_str());

    // run part 2 experiments
    part2();

    // close database
    closeDBPart2();

    // Clear the system
    system("./clearDbs.sh");

    auto stop_exp = chrono::high_resolution_clock::now();

    double elapsedExp = chrono::duration_cast<chrono::duration<double> >(stop_exp - start_exp).count();

    cout << " " << endl;
    cout << "Finished running part 2 experiments in " << elapsedExp / 60 << " minutes." << endl;
    cout << " " << endl;

    cout << "Please give a few seconds for the corresponding plots and csv files to be generated." << endl;
    cout << " " << endl;
}