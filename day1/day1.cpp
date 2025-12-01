#include "aocutils.h"
#include <string>
#include <iostream>
#include <regex>
#include <cmath>

using namespace std;

int main() {
    // Read in the file (and testing some utility functions for later)
    string filename = "input.txt";
    string filestr = file_read(filename);
    vector<string> file = string_split(filestr, '\n');

    /************************ Part 1 ************************/

    unsigned int password = 0;
    int dial_pos = 50;
    int direction = 1;
    int magnitude;

    vector<pair<int, int>> vectors;

    // Loop through and format file contents
    for (int i = 0; i < file.size(); i++) {

        // Determine direction and magnitude of turn
        direction = (file[i].at(0) == 'L') ? -1 : 1;
        magnitude = stoi(regex_replace(file[i], regex("[^0-9]"), ""));

        pair<int, int> vector_value(direction, magnitude);
        vectors.push_back(vector_value);
    }

    for (int i = 0; i < vectors.size(); i++) {
        // Increment dial position by vector
        dial_pos += vectors[i].first * vectors[i].second;

        // Check if divisible by 100
        if ((abs(dial_pos) % 100) == 0) {
            password++;
        }
    }

    std::cout << "Part 1 answer: " << password << '\n';

    /************************ Part 2 ************************/

    password = 0;
    dial_pos = 50;
    int new_dial_pos = 0;

    for (int i = 0; i < vectors.size(); i++) {
        // Increment dial position and copy over to a new value
        new_dial_pos = dial_pos + (vectors[i].first * vectors[i].second);

        // Increment through in the direction, checking every step
        while (dial_pos != new_dial_pos) {
            dial_pos += vectors[i].first;

            // Check if divisible by 100
            if ((abs(dial_pos) % 100) == 0) {
                password++;
            }
        }
    }

    std::cout << "Part 2 answer: " << password << '\n';

    return 0;
}