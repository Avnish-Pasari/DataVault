#ifndef TEXTFILE_H
#define TEXTFILE_H

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <string>
#include "../../util/common.h"
#include "../../util/namespace-std.h"

/**
 * @brief Function to open a text file
 * 
 * @param path The path to the text file
 * @param mode The mode in which the file should be opened
 * 
 * @return FILE* The file pointer to the opened file
 */
FILE *openTextFile(const char *path, const char *mode);

/**
 * @brief Function to close a text file
 * 
 * @param fp The file pointer to the file to be closed
 */
void closeTextFile(FILE *fp);

/**
 * @brief Function to read all names in text file
 * 
 * @param path The path to the text file
 * @param lines The vector to store the names
 */
void readTextFile(const char *path, vector<string> &lines);

/**
 * @brief Function to append a name to a text file on a new line
 * 
 * @param path The path to the text file
 * @param name The name to be appended
 */
void appendNameToTextFile(const char *path, const char *name);

/**
 * @brief Function to check if a name is in a text file
 * 
 * @param path The path to the text file
 * @param name The name to be checked
 * 
 * @return true If the name is in the text file
 * @return false If the name is not in the text file
 */
bool isNameInTextFile(const char *path, const char *name);

#endif // TEXTFILE_H
