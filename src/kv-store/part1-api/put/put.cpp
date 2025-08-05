#include "put.h"

/*
    Flush the contents of the memtable into a new binary file
*/
void flush_to_disk(AVLTree* memtable) {
    // If the memtable is empty, we shouldn't create an empty binary file
    if (memtable->getTreeSize() == 0) {
        return;
    }
    string sstName = "SST_" + to_string(sstNames.size() + 1);
    sstNames.push_back(sstName);

    string fileName = pwd() + "/databases/" + currentOpenDB + "/" + sstName + ".bin";
    FILE* fp = openBinaryFile(fileName.c_str(), "wb");
    if (fp == nullptr) {
        cout << "Error Opening Binary File. Please try again." << endl;
        return;
    }

    // To buffer our insertions and write a page at a time, we will have an output buffer
    KVPair outputbuffer[KV_PAIRS_PER_PAGE];
    int sizeOutputBuffer = 0;
    writeMemtableToBinaryFile(memtable->getRoot(), fp, outputbuffer, sizeOutputBuffer);

    // Write the remaining KVPair objects in the output buffer to the binary file
    writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer);

    closeBinaryFile(fp);

    // Add name of created SST to textfile
    appendNameToTextFile((pwd() + "/databases/" + currentOpenDB + "/file_names.txt").c_str(), sstName.c_str());
}

/*
    Write the nodes of the AVL tree memtable in-order
*/
void writeMemtableToBinaryFile(AVLNode *node, FILE* fp, KVPair* outputbuffer, int &sizeOutputBuffer) {
    if (node == nullptr) {
        return;
    }
    // Traverse left nodes
    writeMemtableToBinaryFile(node->getLeft(), fp, outputbuffer, sizeOutputBuffer);
    // Store the KV pair to write into the output buffer and increment the buffer size
    outputbuffer[sizeOutputBuffer++] = node->getKV();
    // Check if output buffer at capacity
    if (sizeOutputBuffer == KV_PAIRS_PER_PAGE) {
        writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer);
        sizeOutputBuffer = 0;
    }
    // Traverse right nodes
    writeMemtableToBinaryFile(node->getRight(), fp, outputbuffer, sizeOutputBuffer);
}

/*
    Write the input kv-pair to memtable
*/
void put(uint64_t key, int64_t value) {
    // Create KVPair object using input <key, value>
    KVPair nodeToAdd = KVPair(key, value);

    // Check if memtable is at capacity
    if (memtable->getTreeSize() == MEMTABLE_SIZE) {
        flush_to_disk(memtable);
        // Clear memtable ==> Analogous to creating a new memtable after flushing to SST
        memtable->clearTree();

    }
    // Add the node to memtable
    memtable->insert(nodeToAdd);
}