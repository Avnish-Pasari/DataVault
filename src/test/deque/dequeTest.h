#ifndef DEQUE_TEST_H
#define DEQUE_TEST_H

#include "../../kv-store/util/deque/deque.h"
#include "../../kv-store/util/namespace-std.h"
#include <iostream>
#include <cassert>

void testDequeInitialization();
void testDequePushBackAndFront();
void testDequePopFront();
void testDequeErase();
void testDequeMoveToBack();

void dequeTestAll();

#endif // DEQUE_TEST_H