#ifndef SCAN_BTREE_TEST_H
#define SCAN_BTREE_TEST_H

#include "../../../../kv-store/util/namespace-std.h"
#include "../../../../kv-store/avl/avl.h"
#include "../../../../kv-store/part2-api/put/put-part2.h"
#include "../../../../kv-store/part2-api/scan/scan-bTree/scan-bTree.h"
#include "../../../../kv-store/part2-api/db/db-part2.h"
#include "../../../../kv-store/util/global.h"
#include <iostream>
#include <cassert>
#include <vector>

void testScanPart2BTreeKeyExistInSST();
void testScanPart2BTreeAcrossMemtableAndSST();
void testScanPart2BTreeAcrossSSTs();

void scanPart2BTreeTestAll();

#endif  // SCAN_BTREE_TEST_H