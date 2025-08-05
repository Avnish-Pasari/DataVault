#include "./delete.h"

/*
    Delete the key
*/
void deleteKey(uint64_t key) {
    // INT64_MIN denotes a tombstone
    putPart3(key, INT64_MIN);
}