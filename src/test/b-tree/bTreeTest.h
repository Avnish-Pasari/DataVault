#ifndef B_TREE_TEST_H
#define B_TREE_TEST_H

#include "../../kv-store/b-tree/b-tree.h"
#include "../../kv-store/util/namespace-std.h"
#include <iostream>
#include <cassert>

void testFanout3();
void testFanout4();

void testNumKeysLessThanFanout();

void testFlushMemtableToBPlusTreeFanout4();

void bTreeTestAll();

#endif // B_TREE_TEST_H