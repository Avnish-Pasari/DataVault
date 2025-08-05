#ifndef GET_BINARYSEARCH_TEST_H
#define GET_BINARYSEARCH_TEST_H

#include "../../../../kv-store/util/namespace-std.h"
#include "../../../../kv-store/avl/avl.h"
#include "../../../../kv-store/part2-api/put/put-part2.h"
#include "../../../../kv-store/part2-api/get/get-binarySearch/get-binarySearch.h"
#include "../../../../kv-store/part2-api/db/db-part2.h"
#include "../../../../kv-store/util/global.h"
#include <iostream>
#include <cassert>

void testGetPart2BinarySearchKeyExistInSST();
void testGetPart2BinarySearchKeyNotExistInSST();

void getPart2BinarySearchTestAll();

#endif  // GET_BINARYSEARCH_TEST_H