#ifndef AVL_TEST_H
#define AVL_TEST_H

#include "../../kv-store/avl/avl.h"
#include "../../kv-store/util/namespace-std.h"
#include <iostream>
#include <cassert>

void testInsertBalanced();
void testInsertLeftLeft();
void testInsertRightRight();
void testInsertLeftRight();
void testInsertRightLeft();
void testComplexInsert();

void testGetKeyExists();
void testGetKeyDoesNotExist();

void testScanEmpty();
void testScanBothKeysExist();
void testScanStartKeyExists();
void testScanEndKeyExists();
void testScanNeitherKeyExists();

void testClearTree();

void avlTestAll();

#endif