#include "put-part3.h"

/*
    Convert memtable to B-tree and flush the contents of the memtable into a new binary file
*/
void flushToDiskPart3(AVLTree* memtable) {
    int memtableSize = memtable->getTreeSize();
    // If the memtable is empty, we shouldn't create an empty binary file
    if (memtableSize == 0) {
        return;
    }

    Result BTreeData;

    // At this point we know that the memtable is full so we flush it to disk.
    
    // Only case when it might not be full is when we close the db. However, in that
    // case we still flush to disk to save the data.
    BTreeData = flushMemtableIntoSST();
    // 'sstName' is the name of the memtable which was just converted into an SST
    string sstName = BTreeData.sstName;
    lsmNames[0] = sstName;
    
    // Putting name into the LSM Metadata i.e. 'lsmNames'
    // in lsmNames, each index represents the level in the LSM tree
    // Eg - index 0 represents the memtable
    // Eg - index 1 represents the first level of the LSM tree
    // Eg - index 2 represents the second level of the LSM tree and so on and so forth.....
    int index = 1;
    int lsmNamesSize = lsmNames.size();
    for(index = 1; index < lsmNamesSize; index++) {
        if(lsmNames[index] == "") {
            break;
        }
    }

    // We should do compaction till (index - 1) and then for the index iteration, 
    // we should do the compaction once and call the bloomfilter & BTree creation 
    // We should also set name in lsmNames via push_back or access index

    // Doing compactions without creating BTs and BFs
    for(int i = 1; i < index - 1; i++) {
        string tempSstName = lsmNames[i];
        // Always be sure to reset the lsmNames before adding new one
        lsmNames[i] = "";
        // The final name of the SST file after compaction is stored in 'sstName'
        sstName = compactionWithoutDelete(tempSstName, sstName);
    }

    // Doing the final compaction where we are creating the BT and BF
    string tempSstName = lsmNames[index - 1];
    // Always be sure to reset the lsmNames before adding new one
    lsmNames[index - 1] = "";

    // The final name of the SST file after compaction is stored in 'sstName'
    // Adding the final name of the SST file to the lsmNames at the appropriate index/level
    if(index == lsmNamesSize) {
        sstName = compactionBTandBFWithDelete(tempSstName, sstName, index);
        lsmNames.push_back(sstName);
    } else {
        sstName = compactionBTandBFWithoutDelete(tempSstName, sstName, index);
        lsmNames[index] = sstName;
    }

}

/**
 * This compactionWithoutDelete() function is only to be called in the loop till (index - 1).
 * This is because we need to compact all the SSTs till the second last level of the LSM tree.
 * At the last level, we will call the function compactionBTandBFWithDelete() or 
 * compactionBTandBFWithoutDelete() (as the case might be) which will create the BTree and BloomFilter.
 */
string compactionWithoutDelete(string sstOld, string sstNew) {
    KVPair outputbuffer[KV_PAIRS_PER_PAGE];
    int sizeOutputBuffer = 0;

    // Create and open the new SST binary file
    string sstName = "SST" + to_string(sstNameCounter);
    sstNameCounter = sstNameCounter + 1;

    string fileName = pwd() + "/databases/" + currentOpenDB + "/" + sstName + ".bin";
    FILE *fp = openBinaryFile(fileName.c_str(), "wb");

    // Read the two SST files
    string fileNameOld = pwd() + "/databases/" + currentOpenDB + "/" + sstOld + ".bin";
    FILE *fpOld = openBinaryFile(fileNameOld.c_str(), "rb");

    string fileNameNew = pwd() + "/databases/" + currentOpenDB + "/" + sstNew + ".bin";
    FILE *fpNew = openBinaryFile(fileNameNew.c_str(), "rb");

    int totalpagesOld = ceil(static_cast<double>(getFileSize(fpOld)) / PAGE_SIZE);
    int totalpagesNew = ceil(static_cast<double>(getFileSize(fpNew)) / PAGE_SIZE);

    int readtotalpagesOld = 0;
    int readtotalpagesNew = 0;

    // Counters to track the kvpair index being read in a page.
    int i = 0;
    int j = 0;
    KVPair kvPairsOld[KV_PAIRS_PER_PAGE];
    KVPair kvPairsNew[KV_PAIRS_PER_PAGE];
    int numReadOld = readPageFromBinaryFile(fpOld, readtotalpagesOld, kvPairsOld);
    int numReadNew = readPageFromBinaryFile(fpNew, readtotalpagesNew, kvPairsNew);

    while (readtotalpagesOld < totalpagesOld && readtotalpagesNew < totalpagesNew) {

        while(i < numReadOld && j < numReadNew) {

            if(kvPairsOld[i].getKey() < kvPairsNew[j].getKey()) {

                if(sizeOutputBuffer < KV_PAIRS_PER_PAGE) {
                    outputbuffer[sizeOutputBuffer++] = kvPairsOld[i];
                } else {
                    // Write the output buffer to the binary file
                    writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer);
                    sizeOutputBuffer = 0;
                    outputbuffer[sizeOutputBuffer++] = kvPairsOld[i];
                }
                
                i++;

            } else if (kvPairsOld[i].getKey() == kvPairsNew[j].getKey()) {

                if(sizeOutputBuffer < KV_PAIRS_PER_PAGE) {
                    outputbuffer[sizeOutputBuffer++] = kvPairsNew[j];
                } else {
                    // Write the output buffer to the binary file
                    writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer);
                    sizeOutputBuffer = 0;
                    outputbuffer[sizeOutputBuffer++] = kvPairsNew[j];
                }
                     
                i++;
                j++;

            } else {
                
                if(sizeOutputBuffer < KV_PAIRS_PER_PAGE) {
                    outputbuffer[sizeOutputBuffer++] = kvPairsNew[j];
                } else {
                    // Write the output buffer to the binary file
                    writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer);
                    sizeOutputBuffer = 0;
                    outputbuffer[sizeOutputBuffer++] = kvPairsNew[j];
                }
                
                j++;

            }

        }

        if (i == numReadOld) {
            // We have read all the KVPairs for this page in OLD SST. We should read the next page and continue
            readtotalpagesOld++;
            // Reset the counter 'i' since we now read a new page from the OLD SST.
            i = 0;
            numReadOld = readPageFromBinaryFile(fpOld, readtotalpagesOld, kvPairsOld);
        }

        if (j == numReadNew) {
            readtotalpagesNew++;
            numReadNew = readPageFromBinaryFile(fpNew, readtotalpagesNew, kvPairsNew);
            j = 0;
        }

    }


    // First we should continue reading from the index of the current page we stopped at.
    // Following this, we should read the remaining valid pages of the SSTs and write them to the output buffer
    
    // Read current page for old SST (if valid)
    for (int k = i; k < numReadOld; k++) {

        if(sizeOutputBuffer < KV_PAIRS_PER_PAGE) {
            outputbuffer[sizeOutputBuffer++] = kvPairsOld[k];
        } else {
            // Write the output buffer to the binary file
            writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer);
            sizeOutputBuffer = 0;
            outputbuffer[sizeOutputBuffer++] = kvPairsOld[k];
        }
        
    }

    // Read current page for new SST (if valid)
    for (int k = j; k < numReadNew; k++) {
    
        if(sizeOutputBuffer < KV_PAIRS_PER_PAGE) {
            outputbuffer[sizeOutputBuffer++] = kvPairsNew[k];
        } else {
            // Write the output buffer to the binary file
            writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer);
            sizeOutputBuffer = 0;
            outputbuffer[sizeOutputBuffer++] = kvPairsNew[k];
        }

    }

    readtotalpagesOld++;
    readtotalpagesNew++;

    // Read remaining valid pages from old SST
    while(readtotalpagesOld < totalpagesOld) {
        numReadOld = readPageFromBinaryFile(fpOld, readtotalpagesOld, kvPairsOld);
        for(i = 0; i < numReadOld; i++) {

            if(sizeOutputBuffer < KV_PAIRS_PER_PAGE) {
                outputbuffer[sizeOutputBuffer++] = kvPairsOld[i];
            } else {
                // Write the output buffer to the binary file
                writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer);
                sizeOutputBuffer = 0;
                outputbuffer[sizeOutputBuffer++] = kvPairsOld[i];
            }
            
        }
        readtotalpagesOld++;
    }

    // Read remaining valid pages from new SST
    while(readtotalpagesNew < totalpagesNew) {
        numReadNew = readPageFromBinaryFile(fpNew, readtotalpagesNew, kvPairsNew);
        for(j = 0; j < numReadNew; j++) {

            if(sizeOutputBuffer < KV_PAIRS_PER_PAGE) {
                outputbuffer[sizeOutputBuffer++] = kvPairsNew[j];
            } else {
                // Write the output buffer to the binary file
                writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer);
                sizeOutputBuffer = 0;
                outputbuffer[sizeOutputBuffer++] = kvPairsNew[j];
            }
            
        }
        readtotalpagesNew++;
    }

    // Write the remaining KVPair objects in the output buffer to the binary file
    writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer);
    
    closeBinaryFile(fp);
    closeBinaryFile(fpOld);
    closeBinaryFile(fpNew);

    // Delete the 'sstOld' and 'sstNew' SST, BT, BF files

    // Deleting the 'sstOld' and 'sstNew' SSTs
    remove(fileNameOld.c_str());
    remove(fileNameNew.c_str());

    // extracting the number from 'sstOld' name
    int sstOldNum = stoi(sstOld.substr(sstOld.find("T") + 1));
    // extracting the number from 'sstNew' name
    int sstNewNum = stoi(sstNew.substr(sstNew.find("T") + 1));

    // Deleting the BTs
    string btNameOld = "BT" + to_string(sstOldNum);
    string btNameNew = "BT" + to_string(sstNewNum);
    string fileNameOldBT = pwd() + "/databases/" + currentOpenDB + "/" + btNameOld + ".bin";
    string fileNameNewBT = pwd() + "/databases/" + currentOpenDB + "/" + btNameNew + ".bin";

    remove(fileNameOldBT.c_str());
    remove(fileNameNewBT.c_str());

    // Deleting the BFs
    string bfNameOld = "BF" + to_string(sstOldNum);
    string bfNameNew = "BF" + to_string(sstNewNum);
    string fileNameOldBF = pwd() + "/databases/" + currentOpenDB + "/" + bfNameOld + ".bin";
    string fileNameNewBF = pwd() + "/databases/" + currentOpenDB + "/" + bfNameNew + ".bin";

    remove(fileNameOldBF.c_str());
    remove(fileNameNewBF.c_str());

    return sstName; // This is the name of the final SST file created after compaction
}

/**
 * This compactionBTandBFWithDelete() function is only to be called in the loop for the 'index'-th iteration.
 * This is because at the last level we need to create the BTree and BloomFilter.
 * We also get rid of tombstones i.e. deletes in this function.
 */
string compactionBTandBFWithDelete(string sstOld, string sstNew, int level) {
    KVPair outputbuffer[KV_PAIRS_PER_PAGE];
    int sizeOutputBuffer = 0;

    // Create and open the new SST binary file
    string sstName = "SST" + to_string(sstNameCounter);
    sstNameCounter = sstNameCounter + 1;

    string fileName = pwd() + "/databases/" + currentOpenDB + "/" + sstName + ".bin";
    FILE *fp = openBinaryFile(fileName.c_str(), "wb");

    // Read the two SST files
    string fileNameOld = pwd() + "/databases/" + currentOpenDB + "/" + sstOld + ".bin";
    FILE *fpOld = openBinaryFile(fileNameOld.c_str(), "rb");

    string fileNameNew = pwd() + "/databases/" + currentOpenDB + "/" + sstNew + ".bin";
    FILE *fpNew = openBinaryFile(fileNameNew.c_str(), "rb");

    // Initializing (Creating) the BloomFilter for this level
    BloomFilter bF(level);

    // Initializing the BTree (maxKeys) for this level
    int countKeys = 0;
    int countMaxKeys = 0;
    uint64_t maxKeys[(static_cast<int>(pow(2, level))) * PAGES_PER_SST] = {0};

    int totalpagesOld = ceil(static_cast<double>(getFileSize(fpOld)) / PAGE_SIZE);
    int totalpagesNew = ceil(static_cast<double>(getFileSize(fpNew)) / PAGE_SIZE);

    int readtotalpagesOld = 0;
    int readtotalpagesNew = 0;

    // Counters to track the kvpair index being read in a page.
    int i = 0;
    int j = 0;
    KVPair kvPairsOld[KV_PAIRS_PER_PAGE];
    KVPair kvPairsNew[KV_PAIRS_PER_PAGE];
    int numReadOld = readPageFromBinaryFile(fpOld, readtotalpagesOld, kvPairsOld);
    int numReadNew = readPageFromBinaryFile(fpNew, readtotalpagesNew, kvPairsNew);

    while (readtotalpagesOld < totalpagesOld && readtotalpagesNew < totalpagesNew) {

        while(i < numReadOld && j < numReadNew) {

            if(kvPairsOld[i].getKey() < kvPairsNew[j].getKey()) {

                if(kvPairsOld[i].getValue() != INT64_MIN) {
                    if(sizeOutputBuffer < KV_PAIRS_PER_PAGE) {
                        outputbuffer[sizeOutputBuffer++] = kvPairsOld[i];
                    } else {
                        // Write the output buffer to the binary file
                        writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer);
                        sizeOutputBuffer = 0;
                        outputbuffer[sizeOutputBuffer++] = kvPairsOld[i];
                    }

                    bF.insertKey(kvPairsOld[i].getKey());

                    countKeys = countKeys + 1;
                    if(countKeys % KV_PAIRS_PER_PAGE == 0) {
                        countKeys = 0;
                        maxKeys[countMaxKeys++] = kvPairsOld[i].getKey();
                    }
                }
                i++;

            } else if (kvPairsOld[i].getKey() == kvPairsNew[j].getKey()) {

                if(kvPairsNew[j].getValue() != INT64_MIN) {
                    if(sizeOutputBuffer < KV_PAIRS_PER_PAGE) {
                        outputbuffer[sizeOutputBuffer++] = kvPairsNew[j];
                    } else {
                        // Write the output buffer to the binary file
                        writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer);
                        sizeOutputBuffer = 0;
                        outputbuffer[sizeOutputBuffer++] = kvPairsNew[j];
                    }

                    bF.insertKey(kvPairsNew[j].getKey());

                    countKeys = countKeys + 1;
                    if(countKeys % KV_PAIRS_PER_PAGE == 0) {
                        countKeys = 0;
                        maxKeys[countMaxKeys++] = kvPairsNew[j].getKey();
                    }
                }       
                i++;
                j++;

            } else {
                
                if(kvPairsNew[j].getValue() != INT64_MIN) {
                    if(sizeOutputBuffer < KV_PAIRS_PER_PAGE) {
                        outputbuffer[sizeOutputBuffer++] = kvPairsNew[j];
                    } else {
                        // Write the output buffer to the binary file
                        writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer);
                        sizeOutputBuffer = 0;
                        outputbuffer[sizeOutputBuffer++] = kvPairsNew[j];
                    }

                    bF.insertKey(kvPairsNew[j].getKey());

                    countKeys = countKeys + 1;
                    if(countKeys % KV_PAIRS_PER_PAGE == 0) {
                        countKeys = 0;
                        maxKeys[countMaxKeys++] = kvPairsNew[j].getKey();
                    }
                }
                j++;

            }

        }

        if (i == numReadOld) {
            // We have read all the KVPairs for this page in OLD SST. We should read the next page and continue
            readtotalpagesOld++;
            // Reset the counter 'i' since we now read a new page from the OLD SST.
            i = 0;
            numReadOld = readPageFromBinaryFile(fpOld, readtotalpagesOld, kvPairsOld);
        }

        if (j == numReadNew) {
            readtotalpagesNew++;
            numReadNew = readPageFromBinaryFile(fpNew, readtotalpagesNew, kvPairsNew);
            j = 0;
        }

    }


    // First we should continue reading from the index of the current page we stopped at.
    // Following this, we should read the remaining valid pages of the SSTs and write them to the output buffer
    
    // Read current page for old SST (if valid)
    for (int k = i; k < numReadOld; k++) {

        if(kvPairsOld[k].getValue() != INT64_MIN) {
            if(sizeOutputBuffer < KV_PAIRS_PER_PAGE) {
                outputbuffer[sizeOutputBuffer++] = kvPairsOld[k];
            } else {
                // Write the output buffer to the binary file
                writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer);
                sizeOutputBuffer = 0;
                outputbuffer[sizeOutputBuffer++] = kvPairsOld[k];
            }

            bF.insertKey(kvPairsOld[k].getKey());

            countKeys = countKeys + 1;
            if(countKeys % KV_PAIRS_PER_PAGE == 0) {
                countKeys = 0;
                maxKeys[countMaxKeys++] = kvPairsOld[k].getKey();
            }
        }

    }

    // Read current page for new SST (if valid)
    for (int k = j; k < numReadNew; k++) {

        if(kvPairsNew[k].getValue() != INT64_MIN) {
            if(sizeOutputBuffer < KV_PAIRS_PER_PAGE) {
                outputbuffer[sizeOutputBuffer++] = kvPairsNew[k];
            } else {
                // Write the output buffer to the binary file
                writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer);
                sizeOutputBuffer = 0;
                outputbuffer[sizeOutputBuffer++] = kvPairsNew[k];
            }

            bF.insertKey(kvPairsNew[k].getKey());

            countKeys = countKeys + 1;
            if(countKeys % KV_PAIRS_PER_PAGE == 0) {
                countKeys = 0;
                maxKeys[countMaxKeys++] = kvPairsNew[k].getKey();
            }
        }

    }

    readtotalpagesOld++;
    readtotalpagesNew++;

    // Read remaining valid pages from old SST
    while(readtotalpagesOld < totalpagesOld) {
        numReadOld = readPageFromBinaryFile(fpOld, readtotalpagesOld, kvPairsOld);
        for(i = 0; i < numReadOld; i++) {
            if(kvPairsOld[i].getValue() != INT64_MIN) {
                if(sizeOutputBuffer < KV_PAIRS_PER_PAGE) {
                    outputbuffer[sizeOutputBuffer++] = kvPairsOld[i];
                } else {
                    // Write the output buffer to the binary file
                    writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer);
                    sizeOutputBuffer = 0;
                    outputbuffer[sizeOutputBuffer++] = kvPairsOld[i];
                }

                bF.insertKey(kvPairsOld[i].getKey());

                countKeys = countKeys + 1;
                if(countKeys % KV_PAIRS_PER_PAGE == 0) {
                    countKeys = 0;
                    maxKeys[countMaxKeys++] = kvPairsOld[i].getKey();
                }
            }
        }
        readtotalpagesOld++;
    }

    // Read remaining valid pages from new SST
    while(readtotalpagesNew < totalpagesNew) {
        numReadNew = readPageFromBinaryFile(fpNew, readtotalpagesNew, kvPairsNew);
        for(j = 0; j < numReadNew; j++) {
            if(kvPairsNew[j].getValue() != INT64_MIN) {
                if(sizeOutputBuffer < KV_PAIRS_PER_PAGE) {
                    outputbuffer[sizeOutputBuffer++] = kvPairsNew[j];
                } else {
                    // Write the output buffer to the binary file
                    writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer);
                    sizeOutputBuffer = 0;
                    outputbuffer[sizeOutputBuffer++] = kvPairsNew[j];
                }

                bF.insertKey(kvPairsNew[j].getKey());

                countKeys = countKeys + 1;
                if(countKeys % KV_PAIRS_PER_PAGE == 0) {
                    countKeys = 0;
                    maxKeys[countMaxKeys++] = kvPairsNew[j].getKey();
                }
            }
        }
        readtotalpagesNew++;
    }

    // Write the remaining KVPair objects in the output buffer to the binary file
    writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer);
    if (sizeOutputBuffer > 0) {
        maxKeys[countMaxKeys++] = outputbuffer[sizeOutputBuffer - 1].getKey();
    }
    
    closeBinaryFile(fp);
    closeBinaryFile(fpOld);
    closeBinaryFile(fpNew);

    // Delete the 'sstOld' and 'sstNew' SST, BT, BF files

    // Deleting the 'sstOld' and 'sstNew' SSTs
    remove(fileNameOld.c_str());
    remove(fileNameNew.c_str());

    // extracting the number from 'sstOld' name
    int sstOldNum = stoi(sstOld.substr(sstOld.find("T") + 1));
    // extracting the number from 'sstNew' name
    int sstNewNum = stoi(sstNew.substr(sstNew.find("T") + 1));

    // Deleting the BTs
    string btNameOld = "BT" + to_string(sstOldNum);
    string btNameNew = "BT" + to_string(sstNewNum);
    string fileNameOldBT = pwd() + "/databases/" + currentOpenDB + "/" + btNameOld + ".bin";
    string fileNameNewBT = pwd() + "/databases/" + currentOpenDB + "/" + btNameNew + ".bin";

    remove(fileNameOldBT.c_str());
    remove(fileNameNewBT.c_str());

    // Deleting the BFs
    string bfNameOld = "BF" + to_string(sstOldNum);
    string bfNameNew = "BF" + to_string(sstNewNum);
    string fileNameOldBF = pwd() + "/databases/" + currentOpenDB + "/" + bfNameOld + ".bin";
    string fileNameNewBF = pwd() + "/databases/" + currentOpenDB + "/" + bfNameNew + ".bin";

    remove(fileNameOldBF.c_str());
    remove(fileNameNewBF.c_str());

    // Creating the BloomFilter binary file corresponding to 'sstName'
    writeCharArrayToBinaryFile(sstName, bF.getBitmap(), bF.getBitmapSize());

    // Creating the BTree binary file corresponding to 'sstName'
    createBTreeSST(maxKeys, countMaxKeys, sstName);

    return sstName; // This is the name of the final SST file created after compaction
}

/**
 * This compactionBTandBFWithoutDelete() function is only to be called in the loop for the 'index'-th iteration.
 * This is because at the last level we need to create the BTree and BloomFilter.
 * We do NOT get rid of tombstones i.e. deletes in this function.
 */
string compactionBTandBFWithoutDelete(string sstOld, string sstNew, int level) {
    KVPair outputbuffer[KV_PAIRS_PER_PAGE];
    int sizeOutputBuffer = 0;

    // Create and open the new SST binary file
    string sstName = "SST" + to_string(sstNameCounter);
    sstNameCounter = sstNameCounter + 1;

    string fileName = pwd() + "/databases/" + currentOpenDB + "/" + sstName + ".bin";
    FILE *fp = openBinaryFile(fileName.c_str(), "wb");

    // Read the two SST files
    string fileNameOld = pwd() + "/databases/" + currentOpenDB + "/" + sstOld + ".bin";
    FILE *fpOld = openBinaryFile(fileNameOld.c_str(), "rb");

    string fileNameNew = pwd() + "/databases/" + currentOpenDB + "/" + sstNew + ".bin";
    FILE *fpNew = openBinaryFile(fileNameNew.c_str(), "rb");

    // Initializing (Creating) the BloomFilter for this level
    BloomFilter bF(level);

    // Initializing the BTree (maxKeys) for this level
    int countKeys = 0;
    int countMaxKeys = 0;
    uint64_t maxKeys[(static_cast<int>(pow(2, level))) * PAGES_PER_SST] = {0};

    int totalpagesOld = ceil(static_cast<double>(getFileSize(fpOld)) / PAGE_SIZE);
    int totalpagesNew = ceil(static_cast<double>(getFileSize(fpNew)) / PAGE_SIZE);

    int readtotalpagesOld = 0;
    int readtotalpagesNew = 0;

    // Counters to track the kvpair index being read in a page.
    int i = 0;
    int j = 0;
    KVPair kvPairsOld[KV_PAIRS_PER_PAGE];
    KVPair kvPairsNew[KV_PAIRS_PER_PAGE];
    int numReadOld = readPageFromBinaryFile(fpOld, readtotalpagesOld, kvPairsOld);
    int numReadNew = readPageFromBinaryFile(fpNew, readtotalpagesNew, kvPairsNew);

    while (readtotalpagesOld < totalpagesOld && readtotalpagesNew < totalpagesNew) {

        while(i < numReadOld && j < numReadNew) {

            if(kvPairsOld[i].getKey() < kvPairsNew[j].getKey()) {

                if(sizeOutputBuffer < KV_PAIRS_PER_PAGE) {
                    outputbuffer[sizeOutputBuffer++] = kvPairsOld[i];
                } else {
                    // Write the output buffer to the binary file
                    writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer);
                    sizeOutputBuffer = 0;
                    outputbuffer[sizeOutputBuffer++] = kvPairsOld[i];
                }

                bF.insertKey(kvPairsOld[i].getKey());

                countKeys = countKeys + 1;
                if(countKeys % KV_PAIRS_PER_PAGE == 0) {
                    countKeys = 0;
                    maxKeys[countMaxKeys++] = kvPairsOld[i].getKey();
                }
                
                i++;

            } else if (kvPairsOld[i].getKey() == kvPairsNew[j].getKey()) {

                if(sizeOutputBuffer < KV_PAIRS_PER_PAGE) {
                    outputbuffer[sizeOutputBuffer++] = kvPairsNew[j];
                } else {
                    // Write the output buffer to the binary file
                    writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer);
                    sizeOutputBuffer = 0;
                    outputbuffer[sizeOutputBuffer++] = kvPairsNew[j];
                }

                bF.insertKey(kvPairsNew[j].getKey());

                countKeys = countKeys + 1;
                if(countKeys % KV_PAIRS_PER_PAGE == 0) {
                    countKeys = 0;
                    maxKeys[countMaxKeys++] = kvPairsNew[j].getKey();
                }
                       
                i++;
                j++;

            } else {
                
                if(sizeOutputBuffer < KV_PAIRS_PER_PAGE) {
                    outputbuffer[sizeOutputBuffer++] = kvPairsNew[j];
                } else {
                    // Write the output buffer to the binary file
                    writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer);
                    sizeOutputBuffer = 0;
                    outputbuffer[sizeOutputBuffer++] = kvPairsNew[j];
                }

                bF.insertKey(kvPairsNew[j].getKey());

                countKeys = countKeys + 1;
                if(countKeys % KV_PAIRS_PER_PAGE == 0) {
                    countKeys = 0;
                    maxKeys[countMaxKeys++] = kvPairsNew[j].getKey();
                }
                
                j++;

            }

        }

        if (i == numReadOld) {
            // We have read all the KVPairs for this page in OLD SST. We should read the next page and continue
            readtotalpagesOld++;
            // Reset the counter 'i' since we now read a new page from the OLD SST.
            i = 0;
            numReadOld = readPageFromBinaryFile(fpOld, readtotalpagesOld, kvPairsOld);
        }

        if (j == numReadNew) {
            readtotalpagesNew++;
            numReadNew = readPageFromBinaryFile(fpNew, readtotalpagesNew, kvPairsNew);
            j = 0;
        }

    }


    // First we should continue reading from the index of the current page we stopped at.
    // Following this, we should read the remaining valid pages of the SSTs and write them to the output buffer
    
    // Read current page for old SST (if valid)
    for (int k = i; k < numReadOld; k++) {

        if(sizeOutputBuffer < KV_PAIRS_PER_PAGE) {
            outputbuffer[sizeOutputBuffer++] = kvPairsOld[k];
        } else {
            // Write the output buffer to the binary file
            writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer);
            sizeOutputBuffer = 0;
            outputbuffer[sizeOutputBuffer++] = kvPairsOld[k];
        }

        bF.insertKey(kvPairsOld[k].getKey());

        countKeys = countKeys + 1;
        if(countKeys % KV_PAIRS_PER_PAGE == 0) {
            countKeys = 0;
            maxKeys[countMaxKeys++] = kvPairsOld[k].getKey();
        }
        
    }

    // Read current page for new SST (if valid)
    for (int k = j; k < numReadNew; k++) {

        if(sizeOutputBuffer < KV_PAIRS_PER_PAGE) {
            outputbuffer[sizeOutputBuffer++] = kvPairsNew[k];
        } else {
            // Write the output buffer to the binary file
            writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer);
            sizeOutputBuffer = 0;
            outputbuffer[sizeOutputBuffer++] = kvPairsNew[k];
        }

        bF.insertKey(kvPairsNew[k].getKey());

        countKeys = countKeys + 1;
        if(countKeys % KV_PAIRS_PER_PAGE == 0) {
            countKeys = 0;
            maxKeys[countMaxKeys++] = kvPairsNew[k].getKey();
        }
        
    }

    readtotalpagesOld++;
    readtotalpagesNew++;

    // Read remaining valid pages from old SST
    while(readtotalpagesOld < totalpagesOld) {
        numReadOld = readPageFromBinaryFile(fpOld, readtotalpagesOld, kvPairsOld);
        for(i = 0; i < numReadOld; i++) {

            if(sizeOutputBuffer < KV_PAIRS_PER_PAGE) {
                outputbuffer[sizeOutputBuffer++] = kvPairsOld[i];
            } else {
                // Write the output buffer to the binary file
                writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer);
                sizeOutputBuffer = 0;
                outputbuffer[sizeOutputBuffer++] = kvPairsOld[i];
            }

            bF.insertKey(kvPairsOld[i].getKey());

            countKeys = countKeys + 1;
            if(countKeys % KV_PAIRS_PER_PAGE == 0) {
                countKeys = 0;
                maxKeys[countMaxKeys++] = kvPairsOld[i].getKey();
            }
            
        }
        readtotalpagesOld++;
    }

    // Read remaining valid pages from new SST
    while(readtotalpagesNew < totalpagesNew) {
        numReadNew = readPageFromBinaryFile(fpNew, readtotalpagesNew, kvPairsNew);
        for(j = 0; j < numReadNew; j++) {
        
            if(sizeOutputBuffer < KV_PAIRS_PER_PAGE) {
                outputbuffer[sizeOutputBuffer++] = kvPairsNew[j];
            } else {
                // Write the output buffer to the binary file
                writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer);
                sizeOutputBuffer = 0;
                outputbuffer[sizeOutputBuffer++] = kvPairsNew[j];
            }

            bF.insertKey(kvPairsNew[j].getKey());

            countKeys = countKeys + 1;
            if(countKeys % KV_PAIRS_PER_PAGE == 0) {
                countKeys = 0;
                maxKeys[countMaxKeys++] = kvPairsNew[j].getKey();
            }
            
        }
        readtotalpagesNew++;
    }

    // Write the remaining KVPair objects in the output buffer to the binary file
    writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer);
    if (sizeOutputBuffer > 0) {
        maxKeys[countMaxKeys++] = outputbuffer[sizeOutputBuffer - 1].getKey();
    }
    
    closeBinaryFile(fp);
    closeBinaryFile(fpOld);
    closeBinaryFile(fpNew);

    // Delete the 'sstOld' and 'sstNew' SST, BT, BF files

    // Deleting the 'sstOld' and 'sstNew' SSTs
    remove(fileNameOld.c_str());
    remove(fileNameNew.c_str());

    // extracting the number from 'sstOld' name
    int sstOldNum = stoi(sstOld.substr(sstOld.find("T") + 1));
    // extracting the number from 'sstNew' name
    int sstNewNum = stoi(sstNew.substr(sstNew.find("T") + 1));

    // Deleting the BTs
    string btNameOld = "BT" + to_string(sstOldNum);
    string btNameNew = "BT" + to_string(sstNewNum);
    string fileNameOldBT = pwd() + "/databases/" + currentOpenDB + "/" + btNameOld + ".bin";
    string fileNameNewBT = pwd() + "/databases/" + currentOpenDB + "/" + btNameNew + ".bin";

    remove(fileNameOldBT.c_str());
    remove(fileNameNewBT.c_str());

    // Deleting the BFs
    string bfNameOld = "BF" + to_string(sstOldNum);
    string bfNameNew = "BF" + to_string(sstNewNum);
    string fileNameOldBF = pwd() + "/databases/" + currentOpenDB + "/" + bfNameOld + ".bin";
    string fileNameNewBF = pwd() + "/databases/" + currentOpenDB + "/" + bfNameNew + ".bin";

    remove(fileNameOldBF.c_str());
    remove(fileNameNewBF.c_str());

    // Creating the BloomFilter binary file corresponding to 'sstName'
    writeCharArrayToBinaryFile(sstName, bF.getBitmap(), bF.getBitmapSize());

    // Creating the BTree binary file corresponding to 'sstName'
    createBTreeSST(maxKeys, countMaxKeys, sstName);

    return sstName; // This is the name of the final SST file created after compaction
}

/**
 * Create the BTree binary file corresponding to the SST file
 */
void createBTreeSST(uint64_t* keysMax, int numKeysMax, string sstName) {
    KVPair outputbuffer[KV_PAIRS_PER_PAGE];
    int sizeOutputBuffer = 0;

    // Number of internal nodes in the BTree
    int numInternalNodes = 0;
    // internalNodes is an array of KVPair objects where the value is actually the pointer (i.e. page_number)
    KVPair *internalNodes = BTreeUtil::flushMemtableToBPlusTree(keysMax, numKeysMax, numInternalNodes);

    // Create and open BT (BTree) binary file
    string btName = "BT" + to_string(stoi(sstName.substr(sstName.find("T") + 1)));

    string fileName = pwd() + "/databases/" + currentOpenDB + "/" + btName + ".bin";
    FILE *fp = openBinaryFile(fileName.c_str(), "wb");

    // Write internal nodes to binary file
    for (int i = 0; i < numInternalNodes * KV_PAIRS_PER_PAGE; i++) {
        if(sizeOutputBuffer < KV_PAIRS_PER_PAGE) {
            outputbuffer[sizeOutputBuffer++] = internalNodes[i];
        } else {
            // Write the output buffer to the binary file
            writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer);
            sizeOutputBuffer = 0;
            outputbuffer[sizeOutputBuffer++] = internalNodes[i];
        }

    }
    writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer); // causing seek error. 

    // Close BTree binary file
    closeBinaryFile(fp);
}

/**
 * Flush the contents of the memtable into a new binary file
 */
Result flushMemtableIntoSST() {
    KVPair outputbuffer[KV_PAIRS_PER_PAGE];
    int sizeOutputBuffer = 0;

    // Get the size of the memtable
    int memtableSize = memtable->getTreeSize();

    KVPair kvPairs[memtableSize];
    uint64_t keysMax[memtableSize] = {0};
    uint64_t keys[memtableSize] = {0};

    // Index for performing in-order traversal of the memtable recursively
    int index = 0;
    memtableTraversalPart3(memtable->getRoot(), kvPairs, keys, index);

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

    // Create and open the SST binary file
    string sstName = "SST" + to_string(sstNameCounter);
    sstNameCounter = sstNameCounter + 1;

    string fileName = pwd() + "/databases/" + currentOpenDB + "/" + sstName + ".bin";
    FILE *fp = openBinaryFile(fileName.c_str(), "wb");

    // Write KV Pairs to binary file
    for (i = 0; i < memtableSize; i++) {
        if(sizeOutputBuffer < KV_PAIRS_PER_PAGE) {
            outputbuffer[sizeOutputBuffer++] = kvPairs[i];
        } else {
            // Write the output buffer to the binary file
            writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer);
            sizeOutputBuffer = 0;
            outputbuffer[sizeOutputBuffer++] = kvPairs[i];
        }

    }

    writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer);

    closeBinaryFile(fp);

    Result result = {keysMax, idx, sstName};
    return result;
}


/**
 * Traverse memtable in-order (Modified)
 */ 
void memtableTraversalPart3(AVLNode *node, KVPair *kvPairs, uint64_t *keys, int &index) {
    if (node == nullptr) {
        return;
    }
    // Traverse left nodes
    memtableTraversalPart3(node->getLeft(), kvPairs, keys, index);

    // Add KV Pair to kvPairs; Add key to keys
    kvPairs[index] = node->getKV();
    keys[index] = node->getKV().getKey();
    index++;

    // Traverse right nodes
    memtableTraversalPart3(node->getRight(), kvPairs, keys, index);
    return;
}

/*
    Write the input kv-pair to memtable
*/
void putPart3(uint64_t key, int64_t value) {
    // Create KVPair object using input <key, value>
    KVPair nodeToAdd = KVPair(key, value);

    // Check if memtable is at capacity
    if (memtable->getTreeSize() == MEMTABLE_SIZE) {
        flushToDiskPart3(memtable);
        // Clear memtable ==> Analogous to creating a new memtable after flushing to SST
        memtable->clearTree();

    }
    // Add the node to memtable
    memtable->insert(nodeToAdd);
}