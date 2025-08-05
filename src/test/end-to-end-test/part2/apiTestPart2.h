#ifndef API_TEST_PART2_H
#define API_TEST_PART2_H

#include "../../../kv-store/util/namespace-std.h"
#include "../../../kv-store/part2-api/put/put-part2.h"
#include "../../../kv-store/part2-api/get/get-binarySearch/get-binarySearch.h"
#include "../../../kv-store/part2-api/get/get-bTree/get-bTree.h"
#include "../../../kv-store/part2-api/db/db-part2.h"
#include "../../../kv-store/part2-api/scan/scan-binarySearch/scan-binarySearch.h"
#include "../../../kv-store/part2-api/scan/scan-bTree/scan-bTree.h"
#include "../../../kv-store/util/global.h"
#include "../../util/reset.h"

#include <random>
#include <unordered_set>
#include <cstdlib>
#include <iostream>

void testEndToEndAPICallsPart2();

#endif // API_TEST_PART2_H