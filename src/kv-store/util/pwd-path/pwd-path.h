#ifndef PWD_PATH_H
#define PWD_PATH_H

#include <iostream>
#include <string>
#include <unistd.h>
#include <limits.h>
#include "../pwd-path/pwd-path.h"
#include "../../util/namespace-std.h"
#include "../common.h"

// Return path to current working directory as a string upto "/csc443-project"
string pwd();

#endif // PWD_PATH_H
