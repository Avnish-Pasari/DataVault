#ifndef SCAN_BTREE_SEARCH_H
#define SCAN_BTREE_SEARCH_H

#include <cstdint>
#include <iostream>
#include <cassert>
#include <cmath>
#include "../../../util/namespace-std.h"
#include "../../../util/common.h"
#include "../../../util/file-size.h"
#include "../../../util/binary-search.h"
#include "../../../util/pwd-path/pwd-path.h"
#include "../../../avl/avl.h"
#include "../../../disk/binary-file/binary-file.h"
#include "../../../util/global.h"
#include "../../../buffer-pool/buffer-pool.h"

/**
 * @brief Scan for KV pairs from the tree using binary search
 * 
 * @param key1 The start key of the scan
 * @param key2 The end key of the scan
 * 
 * @return vector<KVPair> The vector of KV pairs
 */
vector<KVPair> scan_bTree(uint64_t key1, uint64_t key2);

#endif // SCAN_BTREE_SEARCH_H