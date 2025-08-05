#include "db-part1.h"
 
/**
 * This function takes in a char *name string variable and returns a boolean.
 * It first checks the databases/db_names.txt file to see if the 'name' already exists. 
 * If 'name' exists, the function directly returns true, 
 * else it does the following -
 * 1) it creates a directory with that 'name' in the databases directory.
 * 2) Updates the db_names.txt file with the 'name'
 * 3) Creates an empty file with the name file_names.txt in the newly created directory
 * 
 * Returns:
 * 1 if database is created successfully
 * 2 if database already exists
 * 3 if database is not created successfully i.e. error in creating directory
 */
int openDBPart1(const char *name) {
    // Create directory
    string dir = pwd() + "/databases/" + string(name);
    // Check if directory already exists
    if(isNameInTextFile((pwd() + string("/databases/db_names.txt")).c_str(), name)) {
        cout << "Database " << name << " already exists" << endl;
        // Read existing SST names created for this DB
        string fileName = dir + "/file_names.txt";
        vector<string> lines;
        readTextFile(fileName.c_str(), lines);
        sstNames = lines;
        currentOpenDB = string(name);
        return 2;
    }
    
    if (mkdir(dir.c_str(), 0777) == -1) {
        cout << "Error creating directory: " << dir << endl;
        return 3;
    }

    // Update db_names.txt
    appendNameToTextFile((pwd() + string("/databases/db_names.txt")).c_str(), name);

    // Create file_names.txt
    string file = dir + "/file_names.txt";
    FILE *fp = openTextFile(file.c_str(), "w");
    closeTextFile(fp);
    currentOpenDB = string(name);

    return 1;   
}

/**
 * This function flushes the content of the memtable into an SST and clears any relevant variables. 
 */

void closeDBPart1() {
    flush_to_disk(memtable);
    currentOpenDB = "";
    sstNames.clear();
    memtable->clearTree();
}