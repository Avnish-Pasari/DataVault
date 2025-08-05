#include "get-binarySearch.h"

/*
    Retrieve the value associated with the input key (Buffer Pool Enabled)
    All the traversed pages are added to the buffer pool
    Assume a key cannot be of value 0
*/

int64_t get_binarySearch(uint64_t key) {
    // Check if key is 0
    if (key == 0) {
        return INT64_MIN;
    }    

    // Search for the key in the memtable
    KVPair* kv = memtable->get(key);
    if (kv != nullptr) {
        return kv->getValue();
    }

    // Key not in memtable -- Search for the key in SSTs in youngest to oldest order
    // Use binary search to find the key in the SST
    // Buffer Pool Enabled

    KVPair page[KV_PAIRS_PER_PAGE];  // KVPair array to store the read page

    for (int i = sstNames.size() - 1; i >= 0; i--) {

        string filePath = pwd() + "/databases/" + currentOpenDB + "/" + sstNames[i] + ".bin";
        FILE* fp = openBinaryFile(filePath.c_str(), "rb");
        
        bool pageFound = false;
        long fileSize = getFileSize(fp);
        long num_pages = ceil(static_cast<double>(fileSize) / PAGE_SIZE);

        // 0-indexed page numbers but offset by internal nodes
        // Extract the number of internal nodes from the SST name
        int numInternalNodes = stoi(sstNames[i].substr(sstNames[i].find("-") + 1));
        int leftPageNum = numInternalNodes;    

        int rightPageNum = num_pages - 1;
        int midPageNum;
        int numKVPairsInPage = 0;
        string pageId = "";
        int err = -1;

        // 1. Perform Binary Search to find a probable page where we expect to find the key in
        while (leftPageNum <= rightPageNum) {

            err = -1;

            midPageNum = (leftPageNum + rightPageNum) / 2;

            // Create a pageId
            pageId = sstNames[i] + "-PG" + to_string(midPageNum);

            if(BUFFER_POOL_ENABLED_FLAG) {
                // Check if the page is already in the buffer pool
                err = bufferPool->get(pageId, page, &numKVPairsInPage);

                // When err = 0, page not found in buffer pool
                if (err == 0) {
                    numKVPairsInPage = readPageFromBinaryFile(fp, midPageNum, page);
                    // Add the page to the buffer pool
                    bufferPool->put(pageId, page, numKVPairsInPage);
                }
            } else {
                numKVPairsInPage = readPageFromBinaryFile(fp, midPageNum, page);
            }

            if (numKVPairsInPage == 0) {
                // All pages of this SST are read. Proceed to read another SST
                break;
            }
            // Check the min and max value of the read page to determine if it's a probable page to find key in
            if (key < page[0].getKey()) {
                    rightPageNum = midPageNum - 1;
                }
            else if (key > page[numKVPairsInPage - 1].getKey()) {
                    leftPageNum = midPageNum + 1;
                }
            else if ((page[0].getKey() <= key) && (key <= page[numKVPairsInPage - 1].getKey())){
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
        int64_t idx = KV_BinarySearch(page, numKVPairsInPage, key);
        if (idx != -1) {
            // Close the binary file as we found the key
            closeBinaryFile(fp);
            return page[idx].getValue();
        }

        closeBinaryFile(fp);
    }
    // Key not found
    return INT64_MIN;    
}