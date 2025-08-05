#ifndef CLOSE_TEST_H
#define CLOSE_TEST_H

#include "../../../kv-store/util/namespace-std.h"
#include "../../../kv-store/part1-api/db/db-part1.h"
#include "../../../kv-store/util/global.h"
#include "../../../kv-store/util/file-size.h"
#include "../../../kv-store/disk/binary-file/binary-file.h"
#include <iostream>
#include <cassert>

void testCloseDBWithHalfFilledMemtable();
void testCloseEmptyDB();
void testCloseDBWithInvalidName();

void closeTestAll();

#endif  // CLOSE_TEST_H