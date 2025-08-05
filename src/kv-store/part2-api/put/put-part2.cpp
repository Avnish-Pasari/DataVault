#include "put-part2.h"

/*
    Convert memtable to B-tree and flush the contents of the memtable into a new binary file
*/
void flushToDiskPart2(AVLTree* memtable) {
    int memtableSize = memtable->getTreeSize();
    // If the memtable is empty, we shouldn't create an empty binary file
    if (memtableSize == 0) {
        return;
    }
    
    KVPair kvPairs[memtableSize];
    uint64_t keysMax[memtableSize] = {0};
    uint64_t keys[memtableSize] = {0};
    // Index for performing in-order traversal of the memtable recursively
    int index = 0;
    memtableTraversal(memtable->getRoot(), kvPairs, keys, index);
    // Create B+ tree from sorted keys
    int numInternalNodes = 0;
    // internalNodes is an array of KVPair objects where the value is actually the pointer (i.e. page_number)

    // Creating the array of max keys from each page
    int idx = 0;
    int counter = 0;
    int i = 0;

    for(i = 0; i < memtableSize; i++) {
        if(keys[i] == 0) {
            break;
        }
        counter++;
        if(counter % KV_PAIRS_PER_PAGE == 0) {
            keysMax[idx++] = keys[i];
        }
    }

    if(counter % KV_PAIRS_PER_PAGE != 0) {
        keysMax[idx++] = keys[i - 1];
    }
    
    // keysMax is an array of keys where each key is the maximum key of a page
    // idx is the number of pages
    KVPair *internalNodes = BTreeUtil::flushMemtableToBPlusTree(keysMax, idx, numInternalNodes);

    // Create and open the SST binary file
    string sstName = "SST" + to_string(sstNames.size() + 1) + "-" + to_string(numInternalNodes);
    sstNames.push_back(sstName);

    string fileName = pwd() + "/databases/" + currentOpenDB + "/" + sstName + ".bin";
    FILE* fp = openBinaryFile(fileName.c_str(), "wb");


     // To buffer our insertions and write a page at a time, we will have an output buffer
    KVPair outputBTreebuffer[KV_PAIRS_PER_PAGE];
    int sizeOutputBuffer = 0;

    // Write internal nodes to binary file
    for (i = 0; i < numInternalNodes * KV_PAIRS_PER_PAGE; i++) {
        if(sizeOutputBuffer < KV_PAIRS_PER_PAGE) {
            outputBTreebuffer[sizeOutputBuffer++] = internalNodes[i];
        } else {
            // Write the output buffer to the binary file
            writeKVPairPageToBinaryFile(fp, outputBTreebuffer, sizeOutputBuffer);
            sizeOutputBuffer = 0;
            outputBTreebuffer[sizeOutputBuffer++] = internalNodes[i];
        }
    }

    // Write the remaining KVPair objects in the output buffer to the binary file
    writeKVPairPageToBinaryFile(fp, outputBTreebuffer, sizeOutputBuffer);

    // We will now buffer insertions for writing SST file data
    KVPair outputSSTbuffer[KV_PAIRS_PER_PAGE];
    sizeOutputBuffer = 0;
    // Write KV Pairs to binary file
    for (i = 0; i < memtableSize; i++) {
        if(sizeOutputBuffer < KV_PAIRS_PER_PAGE) {
            outputSSTbuffer[sizeOutputBuffer++] = kvPairs[i];
        } else {
            // Write the output buffer to the binary file
            writeKVPairPageToBinaryFile(fp, outputSSTbuffer, sizeOutputBuffer);
            sizeOutputBuffer = 0;
            outputSSTbuffer[sizeOutputBuffer++] = kvPairs[i];
        }
    }
    
    // Write the remaining KVPair objects in the output buffer to the binary file
    writeKVPairPageToBinaryFile(fp, outputSSTbuffer, sizeOutputBuffer);

    closeBinaryFile(fp);

    // Add name of created SST to textfile
    appendNameToTextFile((pwd() + "/databases/" + currentOpenDB + "/file_names.txt").c_str(), sstName.c_str());
}

/**
 * Traverse memtable in-order (Modified)
 */ 
void memtableTraversal(AVLNode *node, KVPair *kvPairs, uint64_t *keys, int &index) {
    if (node == nullptr) {
        return;
    }
    // Traverse left nodes
    memtableTraversal(node->getLeft(), kvPairs, keys, index);

    // Add KV Pair to kvPairs; Add key to keys
    kvPairs[index] = node->getKV();
    keys[index] = node->getKV().getKey();
    index++;

    // Traverse right nodes
    memtableTraversal(node->getRight(), kvPairs, keys, index);
    return;
}

/*
    Write the input kv-pair to memtable
*/
void putPart2(uint64_t key, int64_t value) {
    // Create KVPair object using input <key, value>
    KVPair nodeToAdd = KVPair(key, value);

    // Check if memtable is at capacity
    if (memtable->getTreeSize() == MEMTABLE_SIZE) {
        flushToDiskPart2(memtable);
        // Clear memtable ==> Analogous to creating a new memtable after flushing to SST
        memtable->clearTree();

    }
    // Add the node to memtable
    memtable->insert(nodeToAdd);
}