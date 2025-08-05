#include "dbTest.h"

// Test openDB
void testOpenDB() {

    int res = openDBPart1("TestDB-1");
    switch (res) {
        case 1:
            cout << "[Test] Success: Database 'TestDB-1' created" << endl;
            break;
        case 2:
            cout << "[Test] Error: Database 'TestDB-1' already exists" << endl;
            break;
        case 3:
            cout << "[Test] Error: Could not create database 'TestDB-1'" << endl;
            break;
    }

    res = openDBPart1("TestDB-1");
    switch (res) {
        case 1:
            cout << "[Test] Error: Database 'TestDB-1' created" << endl;
            break;
        case 2:
            cout << "[Test] Success: Database 'TestDB-1' already exists" << endl;
            break;
        case 3:
            cout << "[Test] Error: Could not create database 'TestDB-1'" << endl;
            break;
    }

    res = openDBPart1("TestDB-2");
    switch (res) {
        case 1:
            cout << "[Test] Success: Database 'TestDB-2' created" << endl;
            break;
        case 2:
            cout << "[Test] Error: Database 'TestDB-2' already exists" << endl;
            break;
        case 3:
            cout << "[Test] Error: Could not create database 'TestDB-2'" << endl;
            break;
    }

    res = openDBPart1("TestDB-1");
    switch (res) {
        case 1:
            cout << "[Test] Error: Database 'TestDB-1' created" << endl;
            break;
        case 2:
            cout << "[Test] Success: Database 'TestDB-1' already exists" << endl;
            break;
        case 3:
            cout << "[Test] Error: Could not create database 'TestDB-1'" << endl;
            break;
    }

    res = openDBPart1("TestDB-2");
    switch (res) {
        case 1:
            cout << "[Test] Error: Database 'TestDB-2' created" << endl;
            break;
        case 2:
            cout << "[Test] Success: Database 'TestDB-2' already exists" << endl;
            break;
        case 3:
            cout << "[Test] Error: Could not create database 'TestDB-2'" << endl;
            break;
    }

}


void dbTestAll() {

    cout << "Running open DB Tests..." << endl << endl;
    testOpenDB();
    cout << "Finished running open DB Tests..." << endl << endl;

    // clean up
    string dir = pwd();

    remove((dir + "/databases/db_names.txt").c_str());
    // Need to create db_names.txt again
    FILE *fp = openTextFile((dir + "/databases/db_names.txt").c_str(), "w");
    closeTextFile(fp);

    remove((dir + "/databases/TestDB-1/file_names.txt").c_str());
    remove((dir + "/databases/TestDB-2/file_names.txt").c_str());
    remove((dir + "/databases/TestDB-1").c_str());
    remove((dir + "/databases/TestDB-2").c_str());
}