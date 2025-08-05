#ifndef _FILESIZE_H_
#define _FILESIZE_H_

#include <stdio.h>
#include <iostream>
#include "./namespace-std.h"

// Given a file pointer, get the size of the file
inline long getFileSize(FILE* fp) {
    if (fp == nullptr) {
        return 0;
    }
    long currentPos = ftell(fp);
    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    // Restore the original position
    fseek(fp, currentPos, SEEK_SET);

    return fileSize;
}

#endif /* _FILESIZE_H_ */