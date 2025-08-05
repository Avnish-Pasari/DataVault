#ifndef PUT_TEST_PART_3_H
#define PUT_TEST_PART_3_H

#include <iostream>
#include <cassert>
#include "../../../kv-store/part3-api/db/db-part3.h"
#include "../../../kv-store/part3-api/put/put-part3.h"
#include "../../../kv-store/part3-api/delete/delete.h"
#include "../../../kv-store/part3-api/get/get-binarySearch/get-binarySearch-part3.h"
#include "../../../kv-store/part3-api/get/get-bTree/get-bTree-part3.h"
#include "../../../kv-store/util/global.h"
#include "../../util/reset.h"

void testFlushToDiskNoCompactionWithoutClosingDB();
void testFlushToDiskWithCompactionWithoutClosingDB();

void testPutInsertionIntoMemtable();
void testPutLargeDataInsertions();

void testUpdatesInMemtable();
void testUpdatesOutsideMemtable();
void testUpdatesDeleteOldKeyValuePair();

void putPart3TestAll();

#endif // PUT_TEST_PART_3_H