#ifndef API_TEST_H
#define API_TEST_H

#include "../../../kv-store/util/namespace-std.h"
#include "../../../kv-store/avl/avl.h"
#include "../../../kv-store/part1-api/put/put.h"
#include "../../../kv-store/part1-api/get/get.h"
#include "../../../kv-store/part1-api/scan/scan.h"
#include "../../../kv-store/part1-api/db/db-part1.h"
#include "../../../kv-store/disk/binary-file/binary-file.h"
#include "../../../kv-store/util/global.h"
#include <iostream>
#include <cassert>

void testEndToEndAPICalls();

void testAllEndToEndAPICalls();

#endif  // API_TEST_H