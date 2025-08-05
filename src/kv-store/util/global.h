#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <vector>
#include <string>
#include "../avl/avl.h"
#include "../buffer-pool/buffer-pool.h"

// All global variables are declared here

extern AVLTree* memtable;
extern string currentOpenDB;
extern vector<string> sstNames;
extern vector<string> lsmNames; // Level 0 is memtable
extern BufferPool* bufferPool;
extern int sstNameCounter;

extern bool BUFFER_POOL_ENABLED_FLAG; // set this to false to disable buffer pool
extern bool BLOOM_FILTER_ENABLED_FLAG; // set this to false to disable bloom filters

#endif /* _GLOBAL_H_ */