#include "scan-binarySearch.h"

/*
    Performs a scan over the input key range returning all kv-pairs
    Assume keys cannot be of value 0
    Buffer Pool Enabled
*/

vector<KVPair> scan_binarySearch(uint64_t key1, uint64_t key2) {
    KVPair page[KV_PAIRS_PER_PAGE];
    bool performSSTBinarySearch = true;
    vector<KVPair> results;

    uint64_t start = min(key1, key2);
    uint64_t end = max(key1, key2);

    // Check if any key is 0
    if (start == 0 || end == 0) {
        return results;
    } 

    // 1. Perform scan over the memtable
    results = memtable->scan(start, end);
    
    // 2. Perform scan over all the SSTs for current DB from youngest to oldest
    for (int i = sstNames.size() - 1; i >= 0; --i) {

        string filePath = pwd() + "/databases/" + currentOpenDB + "/" + sstNames[i] + ".bin";
        FILE* fp = openBinaryFile(filePath.c_str(), "rb");

        long fileSize = getFileSize(fp);
        long num_pages = ceil(static_cast<double>(fileSize) / PAGE_SIZE);

        // 0-indexed page numbers but offset by internal nodes
        // Extract the number of internal nodes from the SST name
        int numInternalNodes = stoi(sstNames[i].substr(sstNames[i].find("-") + 1));
        int leftPageNum = numInternalNodes;

        int rightPageNum = num_pages - 1;

        performSSTBinarySearch = true;
        int probablePageNumber = -1;
        bool readNextSST = false;
        int numKVPairsInPage = 0;
        string pageId = "";
        int err = -1;
        
        while (!readNextSST) {

            // 2.1 Perform Binary Search to find a probable page where we expect to find the start key to begin scan
            while (performSSTBinarySearch && (leftPageNum <= rightPageNum)) {

                err = -1;
                int midPageNum = (leftPageNum + rightPageNum) / 2;

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

            // 2.2 Probable page was not found in this SST so read another SST.
            if (probablePageNumber == -1) {
                break;
            }

            performSSTBinarySearch = false;
            // 2.3 Probable page found. Walk the SST and read all the valid pages
            if (!performSSTBinarySearch) {

                err = -1;
                // Create a pageId
                pageId = sstNames[i] + "-PG" + to_string(probablePageNumber);

                if(BUFFER_POOL_ENABLED_FLAG) {
                    // Check if the page is already in the buffer pool
                    err = bufferPool->get(pageId, page, &numKVPairsInPage);

                    // When err = 0, page not found in buffer pool
                    if (err == 0) {
                        numKVPairsInPage = readPageFromBinaryFile(fp, probablePageNumber, page);
                        // Add the page to the buffer pool
                        bufferPool->put(pageId, page, numKVPairsInPage);
                    }
                } else {
                    numKVPairsInPage = readPageFromBinaryFile(fp, probablePageNumber, page);
                }

                
            }

            // 2.4 Read all valid kv pairs within the scan range
            for (int i = 0; i < numKVPairsInPage; ++i) {
                // Determine stopping condition to check if the page is worth reading or not
                if (end <= page[0].getKey() || start >= page[numKVPairsInPage - 1].getKey()) {
                    readNextSST = true;
                    break;
                }
                if ((page[i].getKey() >= start) && (page[i].getKey() <= end)) {
                    results.push_back(page[i]);
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