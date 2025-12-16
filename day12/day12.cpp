#include "aocutils.h"
#include <string>
#include <iostream>
#include <regex>
#include <unordered_map>
#include <map>

int main() {
    string filename = "day12/input.txt";
    string filestr = file_read(filename);
    vector<string> file = string_split(filestr, '\n');

    /************************ Part 1 ************************/
    string shapes = filestr.substr(0, filestr.find_last_of('#') + 1);
    string areas = filestr.substr(filestr.find_last_of('#') + 1, filestr.length());
    map<int, int> shape_map;
    int curr_shape = 0;
    int answer1 = 0;

    // Get shapes and the total area covered by them
    for (int i = 0; i < shapes.length(); i++) {
        int curr_char = shapes.at(i);

        // [0-9]
        if (curr_char >= 48 && curr_char <= 57) {
            curr_shape = curr_char - 48;
        }

        // #
        if (curr_char == 35) {
            shape_map[curr_shape]++;
        }
    }

    vector<string> regions = string_split(areas, '\n');

    for (int i = 0; i < regions.size(); i++) {
        // Filter invalid lines
        if (regions[i].find(':') < regions[i].length()) {
            vector<string> split = string_split(regions[i], ':');
            
            // Area of the entire region
            vector<string> dims = string_split(split[0], 'x');
            int area_available = stoi(dims[0]) * stoi(dims[1]);

            // Get the amounts of shapes and total area covered so far
            split = string_split(regions[i], ' ');
            int area_covered = 0;
            bool possible = true;

            // Check if the total shape area is greater than the allowed area
            for (int j = 1; j < split.size(); j++) {
                area_covered += (shape_map[j - 1] * stoi(split[j]));

                if (area_covered > area_available) {
                    possible = false;
                    break;
                }
            }

            answer1 += (int)possible;
        }
    }

    std::cout << answer1 << '\n';

    /************************ Part 2 ************************/

    /* AOC 2025 is complete! Only 12 days this year */

    /********************************************************/

    return 2025;
}