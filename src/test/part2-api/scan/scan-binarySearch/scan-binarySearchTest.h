#ifndef SCAN_BINARYSEARCH_TEST_H
#define SCAN_BINARYSEARCH_TEST_H

#include "../../../../kv-store/util/namespace-std.h"
#include "../../../../kv-store/avl/avl.h"
#include "../../../../kv-store/part2-api/put/put-part2.h"
#include "../../../../kv-store/part2-api/scan/scan-binarySearch/scan-binarySearch.h"
#include "../../../../kv-store/part2-api/db/db-part2.h"
#include "../../../../kv-store/util/global.h"
#include <iostream>
#include <cassert>
#include <vector>

void testScanPart2BinarySearchKeyExistInSST();
void testScanPart2BinarySearchAcrossMemtableAndSST();
void testScanPart2BinarySearchAcrossSSTs();

void scanPart2BinarySearchTestAll();

#endif  // SCAN_BINARYSEARCH_TEST_H