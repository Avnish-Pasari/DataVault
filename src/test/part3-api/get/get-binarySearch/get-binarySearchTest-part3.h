#ifndef GET_BINARY_SEARCH_TEST_PART_3_H
#define GET_BINARY_SEARCH_TEST_PART_3_H

#include <iostream>
#include "../../../../kv-store/part3-api/db/db-part3.h"
#include "../../../../kv-store/part3-api/put/put-part3.h"
#include "../../../../kv-store/part3-api/get/get-binarySearch/get-binarySearch-part3.h"
#include "../../../../kv-store/util/global.h"
#include "../../../util/reset.h"

void testKeyInMemtable();
void testKeyInStorage();
void testKeyDoesNotExist();

void testMostRecentKeyInOnlyMemtable();
void testMostRecentKeyInMemtableAndStorage();
void testMostRecentKeyInStorage(); // key exists in multiple levels

void getPart3BinarySearchTestAll();

#endif // GET_BINARY_SEARCH_TEST_PART_3_H