#ifndef GET_BTREE_TEST_PART_3_H
#define GET_BTREE_TEST_PART_3_H

#include <iostream>
#include "../../../../kv-store/part3-api/db/db-part3.h"
#include "../../../../kv-store/part3-api/put/put-part3.h"
#include "../../../../kv-store/part3-api/get/get-bTree/get-bTree-part3.h"
#include "../../../../kv-store/util/global.h"
#include "../../../util/reset.h"

void testKeyInMemtableBTree();
void testKeyInStorageBTree();
void testKeyDoesNotExistBTree();

void testMostRecentKeyInOnlyMemtableBtree();
void testMostRecentKeyInMemtableAndStorageBTree();
void testMostRecentKeyInStorageBTree(); // key exists in multiple levels

void getPart3BTreeTestAll();

#endif // GET_BTREE_TEST_PART_3_H