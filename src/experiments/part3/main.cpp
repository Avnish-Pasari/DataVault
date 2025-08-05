#include "../../kv-store/util/namespace-std.h"
#include "../../kv-store/util/global.h"
#include "../../kv-store/util/common.h"
#include "../../kv-store/disk/text-file/text-file.h"
#include "../../kv-store/part3-api/db/db-part3.h"
#include "../../kv-store/part3-api/put/put-part3.h"
#include "../../kv-store/part3-api/get/get-binarySearch/get-binarySearch-part3.h"
#include "../../kv-store/part3-api/get/get-bTree/get-bTree-part3.h"
#include "../../kv-store/part3-api/scan/scan-binarySearch/scan-binarySearch-part3.h"
#include "../../kv-store/part3-api/scan/scan-bTree/scan-bTree-part3.h"

#include <random>
#include <unordered_set>
#include <chrono>

AVLTree* memtable = new AVLTree();
string currentOpenDB = "";
vector<string> sstNames;
BufferPool* bufferPool = new BufferPool();
vector<string> lsmNames(1, "");
int sstNameCounter = 1;

bool BUFFER_POOL_ENABLED_FLAG = true;
bool BLOOM_FILTER_ENABLED_FLAG = true;

// seed random number generator
random_device rd;
mt19937 gen(1);

void generateUniformlyRandomKeysToInsertPart3(int mb, uint64_t *keys) {
    int numKVPairsToInsert = mb * 65536; // 1MB = 1024KB = 1024 * 1024 Bytes = 1024 * (1024 / 16) = 65536 KV pairs
    
    uniform_int_distribution<uint64_t> keyDis(1, UINT64_MAX);

    for (int i = 0; i < numKVPairsToInsert; i++) {
        keys[i] = keyDis(gen);
    }
}

void generateUniformlyRandomKeysToSearchPart3(int numKeys, uint64_t *keys) {
    
    uniform_int_distribution<uint64_t> keyDis(1, UINT64_MAX);

    for (int i = 0; i < numKeys; i++) {
        keys[i] = keyDis(gen);
    }
}

void generateUniformlyRandomKeysForScanPart3(int numKeys, uint64_t &startKey, uint64_t &endKey) {
    
    uniform_int_distribution<uint64_t> keyDis(1, UINT64_MAX - (numKeys * 2));

    startKey = keyDis(gen);
    endKey = startKey + numKeys - 1; // we have -1 since scan range is inclusive
    
}

void part3() {
    FILE *outfilePut = openTextFile("./experiments/part3/part3-put.txt", "w");
    FILE *outfileGet = openTextFile("./experiments/part3/part3-get.txt", "w");
    FILE *outfileScan = openTextFile("./experiments/part3/part3-scan.txt", "w");

    int oldNumMb = 0;
    for (int i = 0; i <= 10; i++) {

        const int numKeysInExperiment = 7000;
        uint64_t *keysToSearch = new uint64_t[numKeysInExperiment];
        generateUniformlyRandomKeysToSearchPart3(numKeysInExperiment, keysToSearch);

        const int numScanKeysInExperiment = 7000;
        uint64_t startKey = 0;
        uint64_t endKey = 0;
        generateUniformlyRandomKeysForScanPart3(numScanKeysInExperiment, startKey, endKey);

        cout << " " << endl; // Add a new line for better readability in console
        int numMb = 1 << i;
        
        // generate keys to insert
        uint64_t *keys = new uint64_t[(numMb - oldNumMb) * 65536];
        generateUniformlyRandomKeysToInsertPart3((numMb - oldNumMb), keys);

        cout << "Increasing data size to be: " << numMb << " MB" << endl;

        // measure performce for the PUT operation
        cout << "Running PUT to increase data size to: " << numMb << " MB" << endl;

        auto start = chrono::high_resolution_clock::now();
        for (int j = 0; j < (numMb - oldNumMb) * 65536; j++) {
            putPart3(keys[j], 1);
        }
        auto stop = chrono::high_resolution_clock::now();

        double elapsedPut = chrono::duration_cast<chrono::duration<double> >(stop - start).count();
        double throughputPut = static_cast<double>((numMb - oldNumMb) * 65536) / elapsedPut;
        fprintf(outfilePut, "data_size=%d, throughput=%.2f\n", numMb, throughputPut);
        
        cout << "Finished running PUT to increase data size to: " << numMb << " MB" << endl;

        // free memory from keys to insert
        delete[] keys;
        oldNumMb = numMb;
        
        // measure performance for the GET operation (binary search)
        cout << "Running binary search GET operations on data size: " << numMb << " MB" << endl;

        start = chrono::high_resolution_clock::now();
        for (int j = 0; j < numKeysInExperiment; j++) {
            get_binarySearch_part3(keysToSearch[j]);
        }
        stop = chrono::high_resolution_clock::now();

        double elapsedGetBinarySearch = chrono::duration_cast<chrono::duration<double> >(stop - start).count();
        double throughputGetBinarySearch = static_cast<double>(numKeysInExperiment) / elapsedGetBinarySearch;

        cout << "Finished running binary search GET operations on data size: " << numMb << " MB" << endl;

        // measure performance for the GET operation (b-tree)
        cout << "Running B-Tree GET operations on data size: " << numMb << " MB" << endl;

        start = chrono::high_resolution_clock::now();
        for (int j = 0; j < numKeysInExperiment; j++) {
            get_bTree_part3(keysToSearch[j]);
        }
        stop = chrono::high_resolution_clock::now();

        double elapsedGetBTree = chrono::duration_cast<chrono::duration<double> >(stop - start).count();
        double throughputGetBTree = static_cast<double>(numKeysInExperiment) / elapsedGetBTree;
        
        cout << "Finished running B-Tree GET operations on data size: " << numMb << " MB" << endl;
        
        // record results for GET operation
        fprintf(outfileGet, "data_size=%d, throughput_binary_search=%.2f, throughput_b_tree=%.2f\n", numMb, throughputGetBinarySearch, throughputGetBTree);

        // measure performance for the SCAN operation (binary search)
        cout << "Running binary search SCAN operations on data size: " << numMb << " MB" << endl;

        start = chrono::high_resolution_clock::now();
        scan_binarySearch_part3(startKey, endKey);
        stop = chrono::high_resolution_clock::now();

        double elapsedScanBinarySearch = chrono::duration_cast<chrono::duration<double> >(stop - start).count();
        double throughputScanBinarySearch = static_cast<double>(numScanKeysInExperiment) / elapsedScanBinarySearch;
        
        cout << "Finished running binary search SCAN operations on data size: " << numMb << " MB" << endl;

        // measure performance for the SCAN operation (b-tree)
        cout << "Running B-Tree SCAN operations on data size: " << numMb << " MB" << endl;

        start = chrono::high_resolution_clock::now();
        scan_bTree_part3(startKey, endKey);
        stop = chrono::high_resolution_clock::now();

        double elapsedScanBTree = chrono::duration_cast<chrono::duration<double> >(stop - start).count();
        double throughputScanBTree = static_cast<double>(numScanKeysInExperiment) / elapsedScanBTree;
        
        cout << "Finished running B-Tree SCAN operations on data size: " << numMb << " MB" << endl;

        // record results for SCAN operation
        fprintf(outfileScan, "data_size=%d, throughput_binary_search=%.2f, throughput_b_tree=%.2f\n", numMb, throughputScanBinarySearch, throughputScanBTree);

        delete[] keysToSearch;
    }

    closeTextFile(outfilePut);
    closeTextFile(outfileGet);
    closeTextFile(outfileScan);

}

int main() {

    auto start_exp = chrono::high_resolution_clock::now();

    cout << "Starting to run part 3 experiments..." << endl;

    // Clear the system
    system("./clearDbs.sh");

    // open database
    string dbName = "part3-experiments-database";
    openDBPart3(dbName.c_str());

    // run part 3 experiments
    part3();

    // close database
    closeDBPart3();;

    // Clear the system
    system("./clearDbs.sh");

    auto stop_exp = chrono::high_resolution_clock::now();

    double elapsedExp = chrono::duration_cast<chrono::duration<double> >(stop_exp - start_exp).count();

    cout << " " << endl;
    cout << "Finished running part 3 experiments in " << elapsedExp / 60 << " minutes." << endl;
    cout << " " << endl;

    cout << "Please give a few seconds for the corresponding plots and csv files to be generated." << endl;
    cout << " " << endl;
}