#include "aocutils.h"
#include <string>
#include <iostream>
#include <cmath>

using namespace std;

// Finds the greatest digit combination from left to right order
static long long greatest_chars(string line, int n_spaces) {
    vector<int> nums = {};
    long long total = 0;
    unsigned int len = line.length();

    // Create spaces for greatest digits, starting at 0 for comparison
    for (int i = 0; i < n_spaces; i++) {
        nums.push_back(0);
    }

    for (int i = 0; i < len; i++) {
        unsigned int current_digit = line[i] - '0';

        // Go through the spaces from first to last
        for (int j = 0; j < n_spaces; j++) {
            // If there is a bigger digit, and it is in an appropriate position (to scan from left-to-right)
            if ((current_digit > nums[j]) && (i < len - (n_spaces - 1 - j))) {
                nums[j] = current_digit;

                // Since a new highest digit was found, reset the rightmost spaces to 0
                for (int k = j + 1; k < n_spaces; k++) {
                    nums[k] = 0;
                }

                // Break since the current digit has been used
                break;
            }
        }
    }

    // Concatenate and return total
    for (int i = 0; i < n_spaces; i++) {
        total += pow(10, (n_spaces - 1 - i)) * nums[i];
    }

    return total;
}

int main() {
    // Read in the file
    string filename = "day3/input.txt";
    string filestr = file_read(filename);
    vector<string> file = string_split(filestr, '\n');

    /************************ Parts 1 + 2 ************************/
    long long answer1 = 0;
    long long answer2 = 0;

    for (int i = 0; i < file.size(); i++) {
        answer1 += greatest_chars(file[i], 2);
        answer2 += greatest_chars(file[i], 12);
    }

    std::cout << "Part 1 answer: " << answer1 << '\n';
    std::cout << "Part 2 answer: " << answer2 << '\n';

    return 0;
}