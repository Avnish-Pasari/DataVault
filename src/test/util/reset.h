#ifndef TEST_RESET_H
#define TEST_RESET_H

#include <iostream>
#include "../../kv-store/util/global.h"
#include "../../kv-store/util/namespace-std.h"

inline void reset() {
    system("./clearDbs.sh");
    memtable->clearTree();
    lsmNames.clear();
    sstNameCounter = 1;
    delete bufferPool;
    bufferPool = new BufferPool();
}

#endif // TEST_RESET_H