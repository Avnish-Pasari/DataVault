#ifndef API_TEST_PART3_H
#define API_TEST_PART3_H

#include "../../../kv-store/util/namespace-std.h"
#include "../../../kv-store/part3-api/put/put-part3.h"
#include "../../../kv-store/part3-api/get/get-binarySearch/get-binarySearch-part3.h"
#include "../../../kv-store/part3-api/get/get-bTree/get-bTree-part3.h"
#include "../../../kv-store/part3-api/db/db-part3.h"
#include "../../../kv-store/part3-api/scan/scan-binarySearch/scan-binarySearch-part3.h"
#include "../../../kv-store/part3-api/scan/scan-bTree/scan-bTree-part3.h"
#include "../../../kv-store/part3-api/delete/delete.h"
#include "../../../kv-store/util/global.h"
#include "../../util/reset.h"

#include <random>
#include <unordered_set>
#include <cstdlib>
#include <iostream>

void testEndToEndAPICallsPart3();

#endif // API_TEST_PART3_H