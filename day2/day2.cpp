#include "aocutils.h"
#include <string>
#include <iostream>

using namespace std;

// Checks for repetitions in both parts
static bool is_repeated(const string& str, bool part2) {
    int length = str.length();

    // Loop through potential divisors of the number
    for (int i = 1; i <= length / 2; i++) {
        // Cases we can skip - divisor doesn't divide wholly, divisor only needs to be 2 for part 1
        if ((length % i != 0) || (!part2 && ((length / i) % 2 != 0))) { continue; }

        // String is not an even length - skip for part 1 
        if (!part2 && length % 2 != 0) { return 0; }

        // Take the first section of the string
        string part = str.substr(0, i);
        string target = part;

        // Rebuild the string, sending it to target
        for (int j = 1; j < (length / i); j++) {
            target += part;

            // If it already doesn't match, abandon building the rest of the target string
            if (target != str.substr(0, target.length())) {
                break;
            }
        }

        // Rebuilt (target) string matches the number, so it is repeating
        if (target == str) {
            return true;
        }
    }

    return false;
}

int main() {
    // Read in the file
    string filename = "day2/input.txt";
    string filestr = file_read(filename);
    vector<string> file = string_split(filestr, ',');

    /************************ Parts 1 + 2 ************************/
    long long answer1 = 0;
    long long answer2 = 0;

    for (int i = 0; i < file.size(); i++) {
        // Split to get lower-higher pairs
        vector<string> num_pairs = string_split(file[i], '-');

        for (long long j = stoll(num_pairs[0]); j <= stoll(num_pairs[1]); j++) {
            if (is_repeated(to_string(j), false)) { answer1 += j; } // Part 1
            if (is_repeated(to_string(j), true))  { answer2 += j; } // Part 2
        }
    }

    std::cout << "Part 1 answer: " << answer1 << '\n';
    std::cout << "Part 2 answer: " << answer2 << '\n';

    return 0;
}