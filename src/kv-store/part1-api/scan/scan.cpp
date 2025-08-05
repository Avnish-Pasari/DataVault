#include "scan.h"

/*
    Performs a scan over the input key range returning all kv-pairs
*/

vector<KVPair> scan(uint64_t key1, uint64_t key2) {
    KVPair kvPairs[KV_PAIRS_PER_PAGE];
    bool performSSTBinarySearch = true;
    vector<KVPair> results;

    // Check if either key is 0
    if (key1 == 0 || key2 == 0) {
        return results;
    }

    uint64_t start = min(key1, key2);
    uint64_t end = max(key1, key2);
    // 1. Perform scan over the memtable
    results = memtable->scan(start, end);
    
    // 2. Perform scan over all the SSTs for current DB from youngest to oldest
    for (int i = sstNames.size() - 1; i >= 0; --i) {
        string filePath = pwd() + "/databases/" + currentOpenDB + "/" + sstNames[i] + ".bin";
        FILE* fp = openBinaryFile(filePath.c_str(), "rb");

        long fileSize = getFileSize(fp);
        long num_pages = ceil(static_cast<double>(fileSize) / PAGE_SIZE);
        int leftPageNum = 0;    // 0-indexed page numbers
        int rightPageNum = num_pages - 1;

        performSSTBinarySearch = true;
        int probablePageNumber = -1;
        bool readNextSST = false;
        int num_kv_pairs = 0;
        
        while (!readNextSST) {
            // 2.1 Perform Binary Search to find a probable page where we expect to find the start key to begin scan
            while (performSSTBinarySearch && (leftPageNum <= rightPageNum)) {
                int midPageNum = (leftPageNum + rightPageNum) / 2;
                num_kv_pairs = readPageFromBinaryFile(fp, midPageNum, kvPairs);
                // Check if the page range overlaps with the scan range
                if ((kvPairs[num_kv_pairs - 1].getKey() >= start) && (kvPairs[0].getKey() <= end)) {
                    probablePageNumber = midPageNum;  // The found page is a valid page. 
                    // However, we could still find pages that satisfy this condition in the left half
                    rightPageNum = midPageNum - 1;
                    continue;
                }
                else if (start > kvPairs[num_kv_pairs - 1].getKey()) {
                    leftPageNum = midPageNum + 1;  // Search the right half
                    continue;
                }
                else {
                    rightPageNum = midPageNum - 1 ; // Search the left half
                    continue;
                }
            }

            // 2.2 Probable page was not found in this SST so read another SST.
            if (probablePageNumber == -1) {
                break;
            }
            performSSTBinarySearch = false;
            // 2.3 Probable page found. Walk the SST and read all the valid pages
            if (!performSSTBinarySearch) {
                num_kv_pairs = readPageFromBinaryFile(fp, probablePageNumber, kvPairs);
            }
            // 2.4 Read all valid kv pairs within the scan range
            for (int i = 0; i < num_kv_pairs; ++i) {
                // Determine stopping condition to check if the page is worth reading or not
                if (end <= kvPairs[0].getKey() || start >= kvPairs[num_kv_pairs - 1].getKey()) {
                    readNextSST = true;
                    break;
                }
                if ((kvPairs[i].getKey() >= start) && (kvPairs[i].getKey() <= end)) {
                    results.push_back(kvPairs[i]);
                }
            }
            probablePageNumber++;
            // 2.5 Read next SST if we have read all the valid pages for this SST
            if (probablePageNumber > num_pages - 1) {
                readNextSST = true;
                break;
            }
        }
        closeBinaryFile(fp);
    }
    sort(results.begin(), results.end());
    return results;
}