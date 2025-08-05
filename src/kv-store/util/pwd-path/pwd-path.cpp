#include "pwd-path.h"

// Return path to current working directory as a string upto "/csc443-project"
string pwd() {
    char cwd[PATH_LENGTH];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        string path = string(cwd);
        size_t found = path.find("/csc443-project");
        if (found != string::npos) {
            return path.substr(0, found + 15);
        }
    }
    cout << "[PWD-PATH] Error: Could not get path to current working directory" << endl;
    return "";
}