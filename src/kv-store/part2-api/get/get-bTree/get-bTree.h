#ifndef GET_BTREE_SEARCH_H
#define GET_BTREE_SEARCH_H

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
 * @brief Retrieve the value associated with the input key (Buffer Pool Enabled)
 * All the traversed pages are added to the buffer pool
 * Assume a key cannot be of value 0
 * 
 * @param key The key to search for
 * 
 * @return int64_t The value associated with the key
 *         INT64_MIN if the key is not found
 */
int64_t get_bTree(uint64_t key);

#endif // GET_BTREE_SEARCH_H