#ifndef DB_PART_3_H
#define DB_PART_3_H

#include <iostream>
#include <sys/stat.h>
#include "../../disk/text-file/text-file.h"
#include "../../util/namespace-std.h"
#include "../../util/pwd-path/pwd-path.h"
#include "../put/put-part3.h"
#include "../../util/global.h"   

/**
 * @brief Opens a database with the given name.
 *
 * This function takes in a char *name string variable and returns a boolean.
 * It first checks the databases/db_names.txt file to see if the 'name' already exists. 
 * If 'name' exists, the function directly returns true, 
 * else it does the following:
 * 1) Creates a directory with that 'name' in the databases directory.
 * 2) Updates the db_names.txt file with the 'name'.
 * 3) Creates an empty file with the name file_names.txt in the newly created directory.
 * 
 * @param name The name of the database to open.
 * @return int 
 * - 1 if the database is created successfully.
 * - 2 if the database already exists.
 * - 3 if the database is not created successfully (i.e., error in creating directory).
 */
int openDBPart3(const char *name);

/**
 * This function flushes the content of the memtable and clears any relevant variables. 
 */
void closeDBPart3();

#endif // DB_PART_3_H
