#ifndef GET_BINARY_SEARCH_PART_3_H
#define GET_BINARY_SEARCH_PART_3_H

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
#include "../../../bloom-filters/bloom-filters.h"

/**
 * @brief Get the value of the key from the KV store
 * 
 * @param key The key to get
 * @return int64_t The value of the key
 */
int64_t get_binarySearch_part3(uint64_t key);

#endif // GET_BINARY_SEARCH_PART_3_H