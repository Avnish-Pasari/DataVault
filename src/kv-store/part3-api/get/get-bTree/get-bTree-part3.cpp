#include "get-bTree-part3.h"

/*
    Retrieve the value associated with the input key (Buffer Pool Enabled)
    Now, Bloom Filters are also enabled
    Thus, check the Bloom Filter before performing B-Tree Search
    All the traversed pages are added to the buffer pool
    Assume a key cannot be of value 0
*/

int64_t get_bTree_part3(uint64_t key) {
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
    // Use bTree to find the key in the SST
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

        filePath = pwd() + "/databases/" + currentOpenDB + "/" + "BT" + to_string(tempNum) + ".bin";
        fp = openBinaryFile(filePath.c_str(), "rb");
        
        long fileSize = getFileSize(fp);
        long num_pages = ceil(static_cast<double>(fileSize) / PAGE_SIZE);

        // Get the number of internal nodes using the BT file size
        string tempFilePath = pwd() + "/databases/" + currentOpenDB + "/" + "BT" + to_string(tempNum) + ".bin";
        FILE* tempFp = openBinaryFile(tempFilePath.c_str(), "rb");
        long tempFileSize = getFileSize(tempFp);
        closeBinaryFile(tempFp);
        int numInternalNodes = ceil(static_cast<double>(tempFileSize) / PAGE_SIZE);

        int numKVPairsInPage = 0;
        pageId = "";
        err = -1;
        int page_number = 0;

        // 1. Perform B-Tree Search to find a probable page where we expect to find the key in
        // Assume that the root always exists and is at page 0
        while(page_number <= numInternalNodes - 1) {
            err = -1;

            // Create a pageId
            pageId = "BT" + to_string(tempNum) + "-PG" + to_string(page_number);

            if(BUFFER_POOL_ENABLED_FLAG) {
                // Check if the page is already in the buffer pool
                err = bufferPool->get(pageId, page, &numKVPairsInPage);

                // When err = 0, page not found in buffer pool
                if (err == 0) {
                    numKVPairsInPage = readPageFromBinaryFile(fp, page_number, page);
                    // Add the page to the buffer pool
                    bufferPool->put(pageId, page, numKVPairsInPage);
                }
            } else {
                numKVPairsInPage = readPageFromBinaryFile(fp, page_number, page);
            }

            // The page is a <key, page_number> page
            // Find index of <0, page_number> in the page and store it in 'j'
            int j = numKVPairsInPage;
            for(j = numKVPairsInPage - 1; j >= 0; j--) {
                if(page[j].getKey() == 0 && page[j].getValue() != 0) {
                    break;
                }
            }

            // Modified Binary Search
            int left = 0;
            int right = j - 1;

            while(left <= right) {
                int mid = (left + right) / 2;
                if (key <= page[mid].getKey()) {
                    right = mid - 1;
                } else {
                    left = mid + 1;
                }
            }

            // 'index' is the index of the <key, page_number> whose page_number we want to read next
            int index = j;
            if (left < j) {
                index = left;
            }

            page_number = page[index].getValue() - 1;   // Since our pages are 0-indexed
        }

        closeBinaryFile(fp);

        filePath = pwd() + "/databases/" + currentOpenDB + "/" + lsmNames[i] + ".bin";
        fp = openBinaryFile(filePath.c_str(), "rb");


        // 2. Probable page_number found.
        // Check if the page - which is a leaf node - (with this page_number) is already in the buffer pool or not
        // This page_number needs to be adjusted because this is now in the SST file (i.e. we have 
        // split the BT internal nodes into a separate binary file)
        page_number = page_number - numInternalNodes;

        err = -1;
        // Create a pageId
        pageId = lsmNames[i] + "-PG" + to_string(page_number);

        if(BUFFER_POOL_ENABLED_FLAG) {
            // Check if the page is already in the buffer pool
            err = bufferPool->get(pageId, page, &numKVPairsInPage);

            // When err = 0, page not found in buffer pool
            if (err == 0) {
                numKVPairsInPage = readPageFromBinaryFile(fp, page_number, page);
                // Add the page to the buffer pool
                bufferPool->put(pageId, page, numKVPairsInPage);
            }
        } else {
            numKVPairsInPage = readPageFromBinaryFile(fp, page_number, page);
        }

        // 3. Thus we perform binary search on this read page.
        int idx = KV_BinarySearch(page, numKVPairsInPage, key);
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