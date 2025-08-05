#include "text-file-test.h"

// Test openTextFile and closeTextFile
void testOpenAndCloseTextFile() {
    FILE *fp = openTextFile("test.txt", "w");
    cout << "[Test] Success: Opened text file" << endl;
    closeTextFile(fp);
    cout << "[Test] Success: Closed text file" << endl;
}

// Test appendNameToTextFile and isNameInTextFile
void testAppendNameToTextFile() {
    appendNameToTextFile("test.txt", "Avnish.txt");
    cout << "[Test] Success: Appended name 'Avnish.txt' to text file" << endl;

    appendNameToTextFile("test.txt", "Aakash.dat");
    cout << "[Test] Success: Appended name 'Aakash.dat' to text file" << endl;

    appendNameToTextFile("test.txt", "Justin");
    cout << "[Test] Success: Appended name 'Justin' to text file" << endl;

    if(isNameInTextFile("test.txt", "Avnish.txt") == true) {
        cout << "[Test] Success: 'Avnish.txt' found in text file" << endl;
    } else {
        cout << "[Test] Error: 'Avnish.txt' not found in text file" << endl;
    }

    if(isNameInTextFile("test.txt", "name1") == false) {
        cout << "[Test] Success: 'name1' not found in text file" << endl;
    } else {
        cout << "[Test] Error: 'name1' found in text file" << endl;
    }

    if(isNameInTextFile("test.txt", "Aakash.dat") == true) {
        cout << "[Test] Success: 'Aakash.dat' found in text file" << endl;
    } else {
        cout << "[Test] Error: 'Aakash.dat' not found in text file" << endl;
    }

    if(isNameInTextFile("test.txt", "Justin") == true) {
        cout << "[Test] Success: 'Justin' found in text file" << endl;
    } else {
        cout << "[Test] Error: 'Justin' not found in text file" << endl;
    }

    if(isNameInTextFile("test.txt", "name2") == false) {
        cout << "[Test] Success: 'name2' not found in text file" << endl;
    } else {
        cout << "[Test] Error: 'name2' found in text file" << endl;
    }
}

// Test readTextFile
void testReadTextFile() {
    vector<string> lines;
    readTextFile("test.txt", lines);

    if(lines.size() == 3) {
        cout << "[Test] Success: Read 3 lines from text file" << endl;
    } else {
        cout << "[Test] Error: Read " << lines.size() << " lines from text file" << endl;
    }

    if(lines[0] == "Avnish.txt" && lines[1] == "Aakash.dat" && lines[2] == "Justin") {
        cout << "[Test] Success: Read correct lines from text file" << endl;
    } else {
        cout << "[Test] Error: Read incorrect lines from text file" << endl;
    }
}

void textFileTestAll() {

    cout << "RUNNING TEXT FILE TESTS..." << endl << endl;
    testOpenAndCloseTextFile();
    testAppendNameToTextFile();
    testReadTextFile();
    cout << "FINISHED RUNNING TEXT FILE TESTS" << endl << endl;

    // Remove the test file
    remove("test.txt");
}