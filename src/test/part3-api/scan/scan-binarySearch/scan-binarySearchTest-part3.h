#ifndef SCAN_BINARY_SEARCH_TEST_PART_3_H
#define SCAN_BINARY_SEARCH_TEST_PART_3_H

#include <iostream>
#include "../../../../kv-store/part3-api/db/db-part3.h"
#include "../../../../kv-store/part3-api/put/put-part3.h"
#include "../../../../kv-store/part3-api/delete/delete.h"
#include "../../../../kv-store/part3-api/scan/scan-binarySearch/scan-binarySearch-part3.h"
#include "../../../../kv-store/util/common.h"
#include "../../../../kv-store/util/global.h"
#include "../../../../kv-store/util/namespace-std.h"
#include "../../../util/reset.h"

void testScanInMemtablePart3BinarySearch();
void testScanInStoragePart3BinarySearch();
void testScanInMemtableAndStoragePart3BinarySearch();

void testScanEmptyRangePart3BinarySearch();
void testScanOneKeyDoesNotExistPart3BinarySearch();
void testScanBothKeysDoNotExistPart3BinarySearch();

void testScanMostRecentKeyInOnlyMemtablePart3BinarySearch();
void testScanMostRecentKeyInMemtableAndStoragePart3BinarySearch();
void testScanMostRecentKeyInStoragePart3BinarySearch();
void testScanWithDeletedKeysPart3BinarySearch();

void scanPart3BinarySearchTestAll();

#endif // SCAN_BINARY_SEARCH_TEST_PART_3_H