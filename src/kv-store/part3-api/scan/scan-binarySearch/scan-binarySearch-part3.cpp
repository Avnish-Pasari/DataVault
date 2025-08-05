#include "scan-binarySearch-part3.h"

/*
    Performs a scan over the input key range returning all kv-pairs
    Assume keys cannot be of value 0
    Buffer Pool Enabled
*/

vector<KVPair> scan_binarySearch_part3(uint64_t key1, uint64_t key2) {
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

        string filePath = pwd() + "/databases/" + currentOpenDB + "/" + lsmNames[i] + ".bin";
        FILE* fp = openBinaryFile(filePath.c_str(), "rb");

        long fileSize = getFileSize(fp);
        long num_pages = ceil(static_cast<double>(fileSize) / PAGE_SIZE);

        int leftPageNum = 0;
        int rightPageNum = num_pages - 1;

        int probablePageNumber = -1;
        int numKVPairsInPage = 0;
        string pageId = "";
        int err = -1;
        
        // 2.1 Perform Binary Search to find a probable page where we expect to find the start key to begin scan
        while (leftPageNum <= rightPageNum) {

            err = -1;
            int midPageNum = (leftPageNum + rightPageNum) / 2;

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

            // Check if the page range overlaps with the scan range
            if ((page[numKVPairsInPage - 1].getKey() >= start) && (page[0].getKey() <= end)) {
                probablePageNumber = midPageNum;  // The found page is a valid page. 
                // However, we could still find pages that satisfy this condition in the left half
                rightPageNum = midPageNum - 1;
                continue;
            }
            else if (start > page[numKVPairsInPage - 1].getKey()) {
                leftPageNum = midPageNum + 1;  // Search the right half
                continue;
            }
            else {
                rightPageNum = midPageNum - 1 ; // Search the left half
                continue;
            }
        }

        closeBinaryFile(fp);

        // 2.2 Probable page was not found in this SST so read another SST.
        if (probablePageNumber == -1) {
            continue;
        }

        // 2.3 Probable page found. Load data in scanBuffer for this level
        for(int t = 0; t < numKVPairsInPage; t++) {
            scanBuffers[i].page[t] = page[t];
        }
        scanBuffers[i].numKVPairsInPage = numKVPairsInPage;
        scanBuffers[i].currentPageNumber = probablePageNumber;
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