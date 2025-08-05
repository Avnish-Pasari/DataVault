#ifndef PUT_PART2_TEST_H
#define PUT_PART2_TEST_H

#include "../../../kv-store/util/namespace-std.h"
#include "../../../kv-store/avl/avl.h"
#include "../../../kv-store/part2-api/put/put-part2.h"
#include "../../../kv-store/part2-api/get/get-binarySearch/get-binarySearch.h"
#include "../../../kv-store/part2-api/db/db-part2.h"
#include "../../../kv-store/disk/binary-file/binary-file.h"
#include "../../../kv-store/util/global.h"
#include <iostream>
#include <cassert>
#include <vector>

void testPutPart2FlushToSST();

void putPart2TestAll();

#endif  // PUT_PART2_TEST_H