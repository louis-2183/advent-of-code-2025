#pragma once

#ifndef AOCUTILS
#define AOCUTILS

#include <string>
#include <vector>

using namespace std;

// Declarations
string file_read(string filename);
vector<string> string_split(const string& str, char delimiter = ' ');

#endif 