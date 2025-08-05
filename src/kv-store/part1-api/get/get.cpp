#include "get.h"

/*
    Retrieve the value associated with the input key
*/

int64_t get(uint64_t key) {

    // Check if key is 0
    if (key == 0) {
        return INT64_MIN;
    }

    KVPair kvPairs[KV_PAIRS_PER_PAGE];
    // Search for the key in the memtable
    KVPair* kv = memtable->get(key);
    if (kv != nullptr) {
        return kv->getValue();
    }
    // Key not in memtable -- Search for the key in SSTs in youngest to oldest order
    for (int i = sstNames.size() - 1; i >= 0; i--) {
        string filePath = pwd() + "/databases/" + currentOpenDB + "/" + sstNames[i] + ".bin";
        FILE* fp = openBinaryFile(filePath.c_str(), "rb");
        
        bool pageFound = false;
        long fileSize = getFileSize(fp);
        long num_pages = ceil(static_cast<double>(fileSize) / PAGE_SIZE);
        int leftPageNum = 0;    // 0-indexed page numbers
        int rightPageNum = num_pages - 1;
        int midPageNum;
        int num_kv_pairs = 0;

        // 1. Perform Binary Search to find a probable page where we expect to find the key in
        while (leftPageNum <= rightPageNum) {
            midPageNum = (leftPageNum + rightPageNum) / 2;
            num_kv_pairs = readPageFromBinaryFile(fp, midPageNum, kvPairs);
            if (num_kv_pairs == 0) {
                // All pages of this SST are read. Proceed to read another SST
                break;
            }
            // Check the min and max value of the read page to determine if it's a probable page to find key in
            if (key < kvPairs[0].getKey()) {
                    rightPageNum = midPageNum - 1;
                }
            else if (key > kvPairs[num_kv_pairs - 1].getKey()) {
                    leftPageNum = midPageNum + 1;
                }
            else if ((kvPairs[0].getKey() <= key) && (key <= kvPairs[num_kv_pairs - 1].getKey())){
                // Key lies in this page's min-max range.
                pageFound = true;
                break;
            }
            else {
                // Key not found in this page.
                break;
            }
        }
         // 2. Since key is not found in SST, continue reading the next youngest SST
        if (!pageFound) {
            closeBinaryFile(fp);
            continue;;
        }
        
        // 3. Probable page found. Thus we perform binary search on this read page.
        int64_t idx = KV_BinarySearch(kvPairs, num_kv_pairs, key);
        if (idx != -1) {
            // Close the binary file as we found the key
            closeBinaryFile(fp);
            return kvPairs[idx].getValue();
        }

        closeBinaryFile(fp);
    }
    // Key not found
    return INT64_MIN;
}