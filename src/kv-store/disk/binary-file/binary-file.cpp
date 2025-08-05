#include "binary-file.h"

// Function to open a binary file
FILE *openBinaryFile(const char *path, const char *mode) {
    FILE *fp = fopen(path, mode);
    
    if(fp == NULL) {
        cout << "Error opening binary file: " << path << endl;
        exit(1);
    }

    return fp;
}


// Function to close a binary file
void closeBinaryFile(FILE *fp) {
    if(fclose(fp) != 0) {
        cout << "Error closing binary file" << endl;
        exit(1);
    }
}

// Function to write a single KVPair to a binary file; Assume the file is open; does not close the file
void writeSingleKVPairToBinaryFile(FILE *fp, KVPair &kvPair) {
    if(fwrite(&kvPair, sizeof(KVPair), 1, fp) != 1) {
        cout << "Error writing a single KV-Pair to binary file" << endl;
        exit(1);
    }
}

/**
 * Function to read a page from a binary file
 * Assume the file is open; does not close the file
 * Input - file pointer, page number (0-indexed), reference to a vector of KVPair
 * The vector should be of size KV_PAIRS_PER_PAGE but may have fewer elements
 * If the page is full, the vector should have KV_PAIRS_PER_PAGE elements
 * Do error checking -
 * If page is out of bounds, return an empty vector
 * If page is not filled completely, the vector should have the number of elements read
 */
int readPageFromBinaryFile(FILE *fp, int page, KVPair *arr) {

    int count = 0;

    if(fseek(fp, page * KV_PAIRS_PER_PAGE * sizeof(KVPair), SEEK_SET) != 0) {
        // Error occured - page might be out of bounds or other error
        if(feof(fp)) {
            // page is out of bounds
            return 0;
        } else {
            cout << "[KVPage] Error seek-setting fp for reading page from binary file" << endl;
            exit(1);
        }
    }

    count = fread(arr, sizeof(KVPair), KV_PAIRS_PER_PAGE, fp);
    if(count == 0) {
        // Error occured - page might be out of bounds or other error
        if(feof(fp)) {
            // page is out of bounds
            return 0;
        } else {
            cout << "Error reading page from binary file" << endl;
            exit(1);
        }
    }

    return count;
}

// Assuming the size of arr is 4096 and arr is empty
// The function returns the number of elements read as an int
// The function reads the page from the binary file by storing it in arr
// Note: A page is 4kb and can store 4096 characters
int readCharPageFromBinaryFile(FILE *fp, int page, char *arr) {

    int count = 0;

    if(fseek(fp, page * PAGE_SIZE, SEEK_SET) != 0) {
        // Error occured - page might be out of bounds or other error
        if(feof(fp)) {
            // page is out of bounds
            return 0;
        } else {
            cout << "[CharPage] Error seek-setting fp for reading page from binary file" << endl;
            exit(1);
        }
    }

    count = fread(arr, sizeof(char), PAGE_SIZE, fp);
    if(count == 0) {
        // Error occured - page might be out of bounds or other error
        if(feof(fp)) {
            // page is out of bounds
            return 0;
        } else {
            cout << "Error reading page from binary file" << endl;
            exit(1);
        }
    }

    return count;
}

// Function to write a char* array to a binary file
void writeCharArrayToBinaryFile(string sstName, char *arr, int size) {

    string bfName = "BF" + to_string(stoi(sstName.substr(sstName.find("T") + 1)));
    string fileName = pwd() + "/databases/" + currentOpenDB + "/" + bfName + ".bin";
    FILE *fp = openBinaryFile(fileName.c_str(), "wb");

    if(fwrite(arr, sizeof(char), size, fp) != size) {
        cout << "Error writing a char array to binary file" << endl;
        exit(1);
    }

    closeBinaryFile(fp);

}

// Function to write a page of KVPair objects to a binary file
void writeKVPairPageToBinaryFile(FILE *fp, KVPair *arr, int size) {
    if(fwrite(arr, sizeof(KVPair), size, fp) != size) {
        cout << "Error writing a page of KVPair objects to binary file" << endl;
        exit(1);
    }
}

// The below function is used for DEBUGGING and TESTING purposes
// Function to write a KVPair vector to a new binary file - we should NOT use this function in the final implementation
void writeKVPairVectorToBinaryFile(const char *path, vector<KVPair> &kvPairs) {
    FILE *fp = openBinaryFile(path, "wb");
    KVPair outputbuffer[KV_PAIRS_PER_PAGE];
    int sizeOutputBuffer = 0;

    // This function is used to test writeKVPairPageToBinaryFile
    for(int i = 0; i < kvPairs.size(); i++) {
        if(sizeOutputBuffer < KV_PAIRS_PER_PAGE) {
            outputbuffer[sizeOutputBuffer++] = kvPairs[i];
        } else {
            // Write the output buffer to the binary file
            writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer);
            sizeOutputBuffer = 0;
            outputbuffer[sizeOutputBuffer++] = kvPairs[i];
        }
    }

    // Write the remaining KVPair objects in the output buffer to the binary file
    writeKVPairPageToBinaryFile(fp, outputbuffer, sizeOutputBuffer);

    closeBinaryFile(fp);
}




