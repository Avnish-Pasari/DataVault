#ifndef PUT_PART2_H
#define PUT_PART2_H

#include <cstdint>
#include <iostream>
#include <cassert>
#include <cmath>
#include "../../util/namespace-std.h"
#include "../../util/common.h"
#include "../../util/pwd-path/pwd-path.h"
#include "../../avl/avl.h"
#include "../../disk/binary-file/binary-file.h"
#include "../../disk/text-file/text-file.h"
#include "../../util/global.h"
#include "../../b-tree/b-tree.h"

/**
 * @brief Write the input kv-pair to memtable
 * 
 * @param key The key
 * @param value The value
 */
void putPart2(uint64_t key, int64_t value);

/**
 * @brief Flush the content of the memtable to disk
 * 
 * @param memtable The memtable
 */
void flushToDiskPart2(AVLTree* memtable);

/**
 * @brief Traverse memtable in-order (Modified)
 * 
 * @param node The node
 * @param kvPairs The KV pairs
 * @param keys The keys
 * @param index The index
 */
void memtableTraversal(AVLNode *node, KVPair *kvPairs, uint64_t *keys, int &index);

#endif // PUT_PART2_H