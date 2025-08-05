#include "scan-bTree.h"

/*
    Performs a scan over the input key range returning all kv-pairs
    Assume keys cannot be of value 0
    Buffer Pool Enabled
*/

vector<KVPair> scan_bTree(uint64_t key1, uint64_t key2) {

    vector<KVPair> results;

    uint64_t start = min(key1, key2);
    uint64_t end = max(key1, key2);

    uint64_t key = start;

    // Check if any key is 0
    if (start == 0 || end == 0) {
        return results;
    }    

    // 1. Perform scan over the memtable
    results = memtable->scan(start, end);

    // 2. Perform scan over all the SSTs for current DB from youngest to oldest
    // Buffer Pool Enabled

    KVPair page[KV_PAIRS_PER_PAGE];  // KVPair array to store the read page

    for (int i = sstNames.size() - 1; i >= 0; i--) {

        string filePath = pwd() + "/databases/" + currentOpenDB + "/" + sstNames[i] + ".bin";
        FILE* fp = openBinaryFile(filePath.c_str(), "rb");
        
        long fileSize = getFileSize(fp);
        long num_pages = ceil(static_cast<double>(fileSize) / PAGE_SIZE);
        int numInternalNodes = stoi(sstNames[i].substr(sstNames[i].find("-") + 1));
        int numKVPairsInPage = 0;
        string pageId = "";
        int err = -1;
        int page_number = 0;

        // 1. Perform B-Tree Search to find a probable page where we expect to find the start key in
        // Assume that the root always exists and is at page 0
        while(page_number <= numInternalNodes - 1) {
            err = -1;

            // Create a pageId
            pageId = sstNames[i] + "-PG" + to_string(page_number);

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

            page_number = page[index].getValue() - 1; // Since our pages are 0-indexed
        }

        // 2. Begin walking from this leaf node
        while(page_number < num_pages) {

            // 3. Probable page_number found. This page_number is a leaf i.e. a data page.
            // Check if the page - which is a leaf node - (with this page_number) is already in the buffer pool or not
            err = -1;
            // Create a pageId
            pageId = sstNames[i] + "-PG" + to_string(page_number);

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

            bool flagReadNextSST = false;

            for (int k = 0; k < numKVPairsInPage; ++k) {
                // Determine stopping condition to check if the page is worth reading or not
                if (end <= page[0].getKey() || start >= page[numKVPairsInPage - 1].getKey()) {
                    flagReadNextSST = true;
                    break;
                }
                if ((page[k].getKey() >= start) && (page[k].getKey() <= end)) {
                    results.push_back(page[k]);
                }
            }

            page_number++;

            if(flagReadNextSST) {
                break;
            }

        }

        closeBinaryFile(fp);
    }

    sort(results.begin(), results.end());
    return results;    
}
