#ifndef GET_BTREE_TEST_H
#define GET_BTREE_TEST_H

#include "../../../../kv-store/util/namespace-std.h"
#include "../../../../kv-store/avl/avl.h"
#include "../../../../kv-store/buffer-pool/buffer-pool.h"
#include "../../../../kv-store/part2-api/get/get-bTree/get-bTree.h"
#include "../../../../kv-store/part2-api/put/put-part2.h"
#include "../../../../kv-store/part2-api/db/db-part2.h"
#include "../../../../kv-store/util/global.h"
#include <iostream>
#include <cassert>

void testGetKeyExistInBTreeSST();
void testGetKeyNotExistInBTreeSST();

void getBTreeTestAll();

#endif  // GET_BTREE_TEST_H