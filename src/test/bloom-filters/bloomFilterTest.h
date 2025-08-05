#ifndef BLOOM_FILTER_TEST_H
#define BLOOM_FILTER_TEST_H

#include "../../kv-store/bloom-filters/bloom-filters.h"
#include "../../kv-store/util/namespace-std.h"
#include <iostream>
#include <cassert>

void testNumberOfHashFunctions();

void testCalculateBitmapSizeForFirstLevel();
void testCalculateBitmapSizePastFirstLevel();

void testInsertKey();

void testGetKeyBitLocationForHashFunction();

void bloomFilterTestAll();

#endif // BLOOM_FILTER_TEST_H