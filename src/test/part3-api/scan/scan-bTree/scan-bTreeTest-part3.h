#ifndef SCAN_B_TREE_TEST_PART_3_H
#define SCAN_B_TREE_TEST_PART_3_H

#include <iostream>
#include "../../../../kv-store/part3-api/db/db-part3.h"
#include "../../../../kv-store/part3-api/put/put-part3.h"
#include "../../../../kv-store/part3-api/delete/delete.h"
#include "../../../../kv-store/part3-api/scan/scan-bTree/scan-bTree-part3.h"
#include "../../../../kv-store/util/common.h"
#include "../../../../kv-store/util/global.h"
#include "../../../../kv-store/util/namespace-std.h"
#include "../../../util/reset.h"

void testScanInMemtablePart3BTree();
void testScanInStoragePart3BTree();
void testScanInMemtableAndStoragePart3BTree();

void testScanEmptyRangePart3BTree();
void testScanOneKeyDoesNotExistPart3BTree();
void testScanBothKeysDoNotExistPart3BTree();

void testScanMostRecentKeyInOnlyMemtablePart3BTree();
void testScanMostRecentKeyInMemtableAndStoragePart3BTree();
void testScanMostRecentKeyInStoragePart3BTree();
void testScanWithDeletedKeysPart3BTree();

void scanPart3BTreeTestAll();

#endif // SCAN_B_TREE_TEST_PART_3_H