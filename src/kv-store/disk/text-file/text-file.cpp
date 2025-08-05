#include "text-file.h"

// Function to open a text file
FILE *openTextFile(const char *path, const char *mode) {
    FILE *fp = fopen(path, mode);

    if(fp == NULL) {
        cout << "Error opening text file: " << path << endl;
        exit(1);
    }

    return fp;
}

// Function to close a text file
void closeTextFile(FILE *fp) {
    if(fclose(fp) != 0) {
        cout << "Error closing text file" << endl;
        exit(1);
    }
}

// Function to read all names in text file
void readTextFile(const char *path, vector<string> &lines) {
    FILE *fp = openTextFile(path, "r"); // Error checking is done in openTextFile

    char line[NAME_LENGTH];
    while(fgets(line, sizeof(line), fp) != NULL) {
        // Remove newline character
        line[strcspn(line, "\n")] = 0;
        lines.push_back(line);
    }

    closeTextFile(fp); // Error checking is done in closeTextFile

}

// Function to append a name to a text file on a new line
void appendNameToTextFile(const char *path, const char *name) {
    FILE *fp = openTextFile(path, "a"); // Error checking is done in openTextFile

    if(fprintf(fp, "%s\n", name) < 0) {
        cout << "Error appending name to text file" << path << endl;
        exit(1);
    }

    closeTextFile(fp); // Error checking is done in closeTextFile
}

// Function to check if a name is in a text file
bool isNameInTextFile(const char *path, const char *name) {
    FILE *fp = openTextFile(path, "r"); // Error checking is done in openTextFile

    char line[NAME_LENGTH];
    while(fgets(line, sizeof(line), fp) != NULL) {
        // Remove newline character
        line[strcspn(line, "\n")] = 0;

        if(strcmp(line, name) == 0) {
            closeTextFile(fp);
            return true;
        }
    }

    closeTextFile(fp); // Error checking is done in closeTextFile
    return false;
}

