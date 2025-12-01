#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

// Read an input file and return it as a string
string file_read(const string filename = "input.txt") {
    string line;
    ifstream myfile("C:\\Users\\louis\\Desktop\\advent of code 2025\\adventofcode\\"+filename);
    std::stringstream ss;
    ss << myfile.rdbuf();

    // Can't open the file - exit
    if (!myfile.is_open())
    {
        cout << "Unable to open file";
        exit(1);
    }

    return ss.str();
}

// Split a string and put into a vector
vector<string> string_split(const string& str, char delimiter = ' ') {
    vector<string> parts;
    stringstream ss(str);
    string part;

    while (getline(ss, part, delimiter)) {
        parts.push_back(part);
    }

    return parts;
}