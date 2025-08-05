#include "scan-bTree-part3.h"

/*
    Performs a scan over the input key range returning all kv-pairs
    Assume keys cannot be of value 0
    Buffer Pool Enabled
*/

vector<KVPair> scan_bTree_part3(uint64_t key1, uint64_t key2) {
    // Creating 1 page buffer for each level of the LSM
    vector<scanBuffer> scanBuffers(lsmNames.size());

    KVPair page[KV_PAIRS_PER_PAGE];
    vector<KVPair> results;

    uint64_t start = min(key1, key2);
    uint64_t end = max(key1, key2);

    // Check if any key is 0
    if (start == 0 || end == 0) {
        return results;
    } 

    // 1. Perform scan over the memtable
    results = memtable->scan(start, end);
    
    // 2. Perform scan over all the levels of the LSM tree to find the probable start page in each level
    for (int i = 1; i < lsmNames.size(); i++) {

        if(lsmNames[i] == "") {
            // Move to the next level
            continue;
        }

        int err = -1;
        string pageId = "";
        int tempNum = stoi(lsmNames[i].substr(lsmNames[i].find("T") + 1));

        string filePath = pwd() + "/databases/" + currentOpenDB + "/" + "BT" + to_string(tempNum) + ".bin";
        FILE* fp = openBinaryFile(filePath.c_str(), "rb");
        
        long fileSize = getFileSize(fp);
        long num_pages = ceil(static_cast<double>(fileSize) / PAGE_SIZE);

        // Get the number of internal nodes using the BT file size
        int numInternalNodes = ceil(static_cast<double>(num_pages) / PAGE_SIZE);

        int numKVPairsInPage = 0;
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
                if (start <= page[mid].getKey()) {
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

        // 2. Probable page_number found.
        // Check if the page - which is a leaf node - (with this page_number) is already in the buffer pool or not
        // This page_number needs to be adjusted because this is now in the SST file (i.e. we have 
        // split the BT internal nodes into a separate binary file)
        page_number = page_number - numInternalNodes;

        filePath = pwd() + "/databases/" + currentOpenDB + "/" + lsmNames[i] + ".bin";
        fp = openBinaryFile(filePath.c_str(), "rb");

        pageId = lsmNames[i] + "-PG" + to_string(page_number);
        err = -1;

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

        fileSize = getFileSize(fp);
        num_pages = ceil(static_cast<double>(fileSize) / PAGE_SIZE);

        closeBinaryFile(fp);

        // 2.2 Probable page found. Load data in scanBuffer for this level
        for(int t = 0; t < numKVPairsInPage; t++) {
            scanBuffers[i].page[t] = page[t];
        }
        scanBuffers[i].numKVPairsInPage = numKVPairsInPage;
        scanBuffers[i].currentPageNumber = page_number;
        scanBuffers[i].maxpages = num_pages;
        
    }

    int flag = 0;

    // 1. Go through the scan range
    while (true) {

        flag = 0;

        // 2. Traverse all the buffers to find the start.
        // If I cannot find start in any page of the buffers, that means start does not exist
        for(int i = 1; i < lsmNames.size(); i++) {
            // Error checking
            if(lsmNames[i] == "") { // Move to the next level
                continue;
            }
            if(scanBuffers[i].numKVPairsInPage == -1) { // Move to the next level
                continue;
            }
            if(scanBuffers[i].numKVPairsInPage == 0) { // Move to the next level
                continue;
            }

            // 3. For each buffer, traverse through the buffer
            for(int j = 0; j < scanBuffers[i].numKVPairsInPage; j++) {
                if(scanBuffers[i].page[j].getKey() == start) {
                    if(!findKeyInVector(results, start)) {
                        results.push_back(scanBuffers[i].page[j]);
                    }
                    flag = 1; // If flag = 1, that means we have found the start
                    break;
                }
            }

            if(flag == 1) {
                break;
            }
        }

        start++;
        if(start > end) {
            break;
        }

        // 3. Find the next probable page where this next 'start' key could be
        // We need to go through all the buffers and keep incrementing the page till we find the probable page
        for(int k = 1; k < lsmNames.size(); k++) {

            // Error checking
            if(lsmNames[k] == "") { // Move to the next level
                continue;
            }
            if(scanBuffers[k].numKVPairsInPage == -1) { // Move to the next level
                continue;
            }
            if(scanBuffers[k].numKVPairsInPage == 0) { // Move to the next level
                continue;
            }

            string filePath = pwd() + "/databases/" + currentOpenDB + "/" + lsmNames[k] + ".bin";
            FILE* fp = openBinaryFile(filePath.c_str(), "rb");

            // While the new 'start' is greater than the max key in the page, then we need to keep reading the next page
            while(start > scanBuffers[k].page[scanBuffers[k].numKVPairsInPage - 1].getKey()) {
                // Read the next page
                scanBuffers[k].currentPageNumber++;
                if(scanBuffers[k].currentPageNumber >= scanBuffers[k].maxpages) {
                    scanBuffers[k].numKVPairsInPage = 0;
                    break;
                }
                
                string pageId = lsmNames[k] + "-PG" + to_string(scanBuffers[k].currentPageNumber);
                int err = -1;

                if(BUFFER_POOL_ENABLED_FLAG) {
                    // Check if the page is already in the buffer pool
                    err = bufferPool->get(pageId, scanBuffers[k].page, &scanBuffers[k].numKVPairsInPage);

                    // When err = 0, page not found in buffer pool
                    if (err == 0) {
                        scanBuffers[k].numKVPairsInPage = readPageFromBinaryFile(fp, scanBuffers[k].currentPageNumber, scanBuffers[k].page);
                        // Add the page to the buffer pool
                        bufferPool->put(pageId, scanBuffers[k].page, scanBuffers[k].numKVPairsInPage);
                    }
                } else {
                    scanBuffers[k].numKVPairsInPage = readPageFromBinaryFile(fp, scanBuffers[k].currentPageNumber, scanBuffers[k].page);
                }
                
            }

            closeBinaryFile(fp);

        }
               
    }

    sort(results.begin(), results.end());

    for(int i = 0; i <results.size(); i++) {
        if(results[i].getValue() == INT64_MIN) {
            results.erase(results.begin() + i);
            i--;
        }
    }

    return results;
}