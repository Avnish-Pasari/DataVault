#ifndef BINARY_FILE_H
#define BINARY_FILE_H

#include <stdio.h>
#include <iostream>
#include <vector>
#include "../../util/common.h"
#include "../../util/pwd-path/pwd-path.h"
#include "../../util/global.h"

#include "../../util/namespace-std.h"

/**
 * @brief Function to open a binary file
 * 
 * @param path The path to the binary file
 * @param mode The mode in which the file should be opened
 * 
 * @return FILE* The file pointer to the opened file
 */
FILE *openBinaryFile(const char *path, const char *mode);

/**
 * @brief Function to close a binary file
 * 
 * @param fp The file pointer to the file to be closed
 */
void closeBinaryFile(FILE *fp);

/**
 * @brief Function to write a single KVPair to a binary file; Assume the file is open; does not close the file
 * 
 * @param fp The file pointer to the binary file
 * @param kvPair The KVPair object to be written to the binary file
 */
void writeSingleKVPairToBinaryFile(FILE *fp, KVPair &kvPair);

/**
 * @brief Function to write a char* array to a binary file
 * 
 * @param sstName The name of the SST file
 * @param arr The char* array to be written to the binary file
 * @param size The size of the char* array
 */
void writeCharArrayToBinaryFile(string sstName, char *arr, int size);

/**
 * @brief Function to write a page of KVPair objects to a binary file
 * 
 * @param fp The file pointer to the binary file
 * @param arr The array of KVPair objects to be written to the binary file
 * @param size The size of the array
 */
void writeKVPairPageToBinaryFile(FILE *fp, KVPair *arr, int size);

/**
 * @brief Function to read a page from a binary file
 * 
 * @param fp The file pointer to the binary file
 * @param page The page number (0-indexed)
 * @param arr The array of KVPair objects to store the read data
 * 
 * @return int The number of elements read
 */
int readPageFromBinaryFile(FILE *fp, int page, KVPair *arr);

/**
 * @brief Function to read a page from a binary file
 * 
 * @param fp The file pointer to the binary file
 * @param page The page number (0-indexed)
 * @param arr The array of char to store the read data
 * 
 * @return int The number of elements read
 */
int readCharPageFromBinaryFile(FILE *fp, int page, char *arr);

/**
 * @brief Function to write a vector of KVPair objects to a binary file
 * 
 * @param path The path to the binary file
 * @param kvPairs The vector of KVPair objects to be written to the binary file
 */
void writeKVPairVectorToBinaryFile(const char *path, vector<KVPair> &kvPairs);

#endif // BINARY_FILE_H
