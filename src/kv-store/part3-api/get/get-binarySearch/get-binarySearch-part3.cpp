#include "get-binarySearch-part3.h"

/*
    Retrieve the value associated with the input key (Buffer Pool Enabled)
    Now, Bloom Filters are also enabled
    Thus, check the Bloom Filter before performing binary search
    All the traversed pages are added to the buffer pool
    Assume a key cannot be of value 0
*/

int64_t get_binarySearch_part3(uint64_t key) {
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
    // Bloom Filters Enabled

    KVPair page[KV_PAIRS_PER_PAGE];  // KVPair array to store the read page
    char bfPage[PAGE_SIZE]; // Char array to store the Bloom Filter page

    for (int i = 1; i < lsmNames.size(); i++) {

        if(lsmNames[i] == "") {
            // Move to the next level
            continue;
        }

        int err = -1;
        string pageId = "";
        int tempNum = stoi(lsmNames[i].substr(lsmNames[i].find("T") + 1));
        int bfPageNumber = 0;
        int bfPageOffset = 0;
        int bfPageSize = 0;
        int bloomFilterFlag = 0;
        string filePath = pwd() + "/databases/" + currentOpenDB + "/" + "BF" + to_string(tempNum) + ".bin";
        FILE* fp = openBinaryFile(filePath.c_str(), "rb");

        // START OF BLOOM FILTER CHECK

        if(BLOOM_FILTER_ENABLED_FLAG) {

            for(int b = 1; b <= BLOOM_FILTER_NUM_HASH_FUNCTIONS; b++) {
                
                BloomFilter::getKeyBitLocationForHashFunction(key, i, b, bfPageNumber, bfPageOffset);
                // Create a pageId
                pageId = "BF" + to_string(tempNum) + "-PG" + to_string(bfPageNumber);

                if(BUFFER_POOL_ENABLED_FLAG) {
                    // Check if the page is already in the buffer pool
                    err = bufferPool->getBfPage(pageId, bfPage, &bfPageSize);

                    // When err = 0, page not found in buffer pool
                    if (err == 0) {
                        bfPageSize = readCharPageFromBinaryFile(fp, bfPageNumber, bfPage);
                        // Add the page to the buffer pool
                        bufferPool->putBfPage(pageId, bfPage, bfPageSize);
                    }
                } else {
                    bfPageSize = readCharPageFromBinaryFile(fp, bfPageNumber, bfPage);
                }

                if(bfPage[bfPageOffset] == '0') { // The key is not present in the Bloom Filter
                    bloomFilterFlag = 1;
                    break;
                }

            }
            closeBinaryFile(fp);

            if(bloomFilterFlag == 1) {
                // Key not found in Bloom Filter. Continue to the next SST
                bloomFilterFlag = 0;
                continue;
            }

        }

        // END OF BLOOM FILTER CHECK

        filePath = pwd() + "/databases/" + currentOpenDB + "/" + lsmNames[i] + ".bin";
        fp = openBinaryFile(filePath.c_str(), "rb");

        bool pageFound = false;
        long fileSize = getFileSize(fp);
        long num_pages = ceil(static_cast<double>(fileSize) / PAGE_SIZE);

        int leftPageNum = 0;    
        int rightPageNum = num_pages - 1;
        int midPageNum;
        int numKVPairsInPage = 0;

        // 1. Perform Binary Search to find a probable page where we expect to find the key in
        while (leftPageNum <= rightPageNum) {

            err = -1;

            midPageNum = (leftPageNum + rightPageNum) / 2;

            // Create a pageId
            pageId = lsmNames[i] + "-PG" + to_string(midPageNum);

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