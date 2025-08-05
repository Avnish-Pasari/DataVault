#ifndef PUT_H
#define PUT_H

#include <cstdint>
#include <iostream>
#include <cassert>
#include <cmath>
#include "../../util/namespace-std.h"
#include "../../util/common.h"
#include "../../util/pwd-path/pwd-path.h"
#include "../../avl/avl.h"
#include "../../disk/binary-file/binary-file.h"
#include "../../disk/text-file/text-file.h"
#include "../../util/global.h"

/**
 * @brief Write a KV pair to the memtable
 * 
 * @param key The key of the KV pair
 * @param value The value of the KV pair
 */
void put(uint64_t key, int64_t value);

/**
 * @brief Flush the contents of the memtable into a new binary file
 * 
 * @param memtable The AVL tree representing the memtable
 */
void flush_to_disk(AVLTree* memtable);

/**
 * @brief Write the nodes of the AVL tree memtable in-order
 * 
 * @param node The root of the AVL tree
 * @param fp The file pointer to the binary file
 * @param outputbuffer The output buffer to store KV pairs
 * @param sizeOutputBuffer The size of the output buffer
 */
void writeMemtableToBinaryFile(AVLNode *node, FILE* fp, KVPair* outputbuffer, int &sizeOutputBuffer);

#endif // PUT_H