#ifndef SCAN_BTREE_SEARCH_PART_3_H
#define SCAN_BTREE_SEARCH_PART_3_H

#include <cstdint>
#include <iostream>
#include <cassert>
#include <set>
#include <cmath>
#include <algorithm>
#include "../../../util/namespace-std.h"
#include "../../../util/common.h"
#include "../../../util/file-size.h"
#include "../../../util/binary-search.h"
#include "../../../util/pwd-path/pwd-path.h"
#include "../../../util/find-key.h"
#include "../../../util/scan-buffer.h"
#include "../../../avl/avl.h"
#include "../../../disk/binary-file/binary-file.h"
#include "../../../util/global.h"
#include "../../../buffer-pool/buffer-pool.h"
#include "../../db/db-part3.h"

/**
 * @brief Performs a scan over the input key range returning all kv-pairs
 * Assume keys cannot be of value 0
 * Buffer Pool Enabled
 * 
 * @param key1 The first (start) key
 * @param key2 The second (end) key
 * @return vector<KVPair> The vector of KV pairs
 */
vector<KVPair> scan_bTree_part3(uint64_t key1, uint64_t key2);

#endif // SCAN_BTREE_SEARCH_PART_3_H