#ifndef DELETE_TEST_PART_3_H
#define DELETE_TEST_PART_3_H

#include <iostream>
#include <cassert>
#include "../../../kv-store/part3-api/db/db-part3.h"
#include "../../../kv-store/part3-api/put/put-part3.h"
#include "../../../kv-store/part3-api/delete/delete.h"
#include "../../../kv-store/part3-api/get/get-binarySearch/get-binarySearch-part3.h"
#include "../../../kv-store/part3-api/get/get-bTree/get-bTree-part3.h"
#include "../../../kv-store/util/global.h"
#include "../../util/reset.h"

void testDeletesInMemtable();
void testDeletesInStorage();
void testDeletesAndReinsertion();

void deletePart3TestAll();

#endif