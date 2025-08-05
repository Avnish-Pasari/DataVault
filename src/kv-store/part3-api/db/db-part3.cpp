#include "db-part3.h"
 
/**
 * This function takes in a char *name string variable and returns an integer.
 * It first checks the databases/db_names.txt file to see if the 'name' already exists. 
 * If 'name' exists, the function directly returns 2, 
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
int openDBPart3(const char *name) {
    // Create directory
    string dir = pwd() + "/databases/" + string(name);

    // Check if directory already exists
    if(isNameInTextFile((pwd() + string("/databases/db_names.txt")).c_str(), name)) {

        cout << "Database " << name << " already exists" << endl;

        // Read existing LSM names created for this DB
        string fileName = dir + "/file_names.txt";

        vector<string> lines;
        readTextFile(fileName.c_str(), lines);

        lsmNames = lines;

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

    if(lsmNames.size() == 0) {
        lsmNames.push_back("");
    }

    currentOpenDB = string(name);

    return 1;   
}

/**
 * This function flushes the content of the memtable and clears any relevant variables. 
 */
void closeDBPart3() {
    flushToDiskPart3(memtable);

    // Overwrite the file_names.txt with the new lsmNames
    string fileName = pwd() + "/databases/" + currentOpenDB + "/file_names.txt";

    if(remove(fileName.c_str()) != 0) {
        cout << "Error removing file with name = " << fileName << endl;
    }

    FILE *fp = openTextFile(fileName.c_str(), "w");
    closeTextFile(fp);

    for(int i = 0; i < lsmNames.size(); i++) {
        appendNameToTextFile(fileName.c_str(), lsmNames[i].c_str());
    }

    currentOpenDB = "";
    lsmNames.clear();
    memtable->clearTree();
}