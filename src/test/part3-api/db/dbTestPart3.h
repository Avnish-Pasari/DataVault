#ifndef DB_TEST_PART_3_H
#define DB_TEST_PART_3_H

#include <iostream>
#include <cassert>
#include <fstream>
#include "../../../kv-store/part3-api/db/db-part3.h"
#include "../../../kv-store/part3-api/get/get-binarySearch/get-binarySearch-part3.h"
#include "../../../kv-store/part3-api/get/get-bTree/get-bTree-part3.h"
#include "../../../kv-store/disk/text-file/text-file.h"
#include "../../../kv-store/disk/binary-file/binary-file.h"
#include "../../../kv-store/util/file-size.h"
#include "../../../kv-store/util/global.h"
#include "../../util/reset.h"

void testOpenDBPart3();

void testCloseDBHalfFilledMemtable();
void testCloseDBForceCompaction();

void validateSSTFile(string sstName, int expectedNumKVPairs);

void dbPart3TestAll();

#endif // DB_TEST_PART_3_H