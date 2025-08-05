#ifndef BINARY_FILE_TEST_H
#define BINARY_FILE_TEST_H

#include "../../../kv-store/disk/binary-file/binary-file.h"

// Helper function that creates a KVPair vector of the given size
void helperCreateKVPairVector(int size, vector<KVPair> &kvPairs);

// Function to test writing to a binary file
void testWriteToBinaryFile();

// Function to test reading from a binary file
void testReadFromBinaryFile();

// Function to test writing and reading a char page to a binary file
void testWriteReadCharPageToBinaryFile();

// Function to test writing and reading multiple char pages to a binary file
void testWriteReadCharMultipleCharToBinaryFile();

// Function to run all binary file tests
void binaryFileTestAll();

#endif // BINARY_FILE_TEST_H
