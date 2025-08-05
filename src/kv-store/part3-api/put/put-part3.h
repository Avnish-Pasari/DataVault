#ifndef PUT_PART3_H
#define PUT_PART3_H

#include <cstdint>
#include <iostream>
#include <cassert>
#include <cmath>
#include <tuple>
#include <functional>
#include <string>
#include "../../util/namespace-std.h"
#include "../../util/file-size.h"
#include "../../util/common.h"
#include "../../util/pwd-path/pwd-path.h"
#include "../../avl/avl.h"
#include "../../disk/binary-file/binary-file.h"
#include "../../disk/text-file/text-file.h"
#include "../../util/global.h"
#include "../../b-tree/b-tree.h"
#include "../../bloom-filters/bloom-filters.h"

struct Result {
    uint64_t* keysMax;
    int numKeysMax;
    string sstName;
};

/**
 * @brief Write a KV pair to the memtable
 * 
 * @param key The key of the KV pair
 * @param value The value of the KV pair
 */
void putPart3(uint64_t key, int64_t value);

/**
 * @brief Flush the contents of the memtable into a new binary file
 * 
 * @param memtable The AVL tree representing the memtable
 */
void flushToDiskPart3(AVLTree* memtable);

/**
 * @brief Traverse the memtable in-order
 * 
 * @param node The node to start the traversal from
 * @param kvPairs The array of KV pairs is stored here
 * @param keys The array of keys is stored here
 * @param index The index
 */
void memtableTraversalPart3(AVLNode *node, KVPair *kvPairs, uint64_t *keys, int &index);

/**
 * @brief Compaction of two SST files without delete
 * Does NOT create the corresponding Bloom Filter and B+ Tree
 * 
 * @param sstOld The name of the old SST file
 * @param sstNew The name of the new SST file
 * 
 * @return string The name of the newly created SST file
 */
string compactionWithoutDelete(string sstOld, string sstNew);

/**
 * @brief Compaction of two SST files with delete
 * It creates the corresponding Bloom Filter and B+ Tree
 * 
 * @param sstOld The name of the old SST file
 * @param sstNew The name of the new SST file
 * @param level The level of the SST file
 * 
 * @return string The name of the newly created SST file
 */
string compactionBTandBFWithDelete(string sstOld, string sstNew, int level);

/**
 * @brief Compaction of two SST files without delete
 * It creates the corresponding Bloom Filter and B+ Tree
 * 
 * @param sstOld The name of the old SST file
 * @param sstNew The name of the new SST file
 * @param level The level of the SST file
 * 
 * @return string The name of the newly created SST file
 */
string compactionBTandBFWithoutDelete(string sstOld, string sstNew, int level);

/**
 * @brief Create a B+ tree binary file from the sorted array of keys 'keysMax' 
 * and the number of pages 'numKeysMax'
 * 
 * @param keysMax The sorted array of the maximum keys in each page of the SST
 * @param numKeysMax The number of pages
 * @param sstName The name of the SST file
 * 
 */
void createBTreeSST(uint64_t* keysMax, int numKeysMax, string sstName);

/**
 * @brief Flush the memtable into the SST
 * 
 * @return Result The result of the flush
 */
Result flushMemtableIntoSST();

#endif // PUT_PART3_H