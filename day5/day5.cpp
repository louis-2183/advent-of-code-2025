#include "aocutils.h"
#include <string>
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

int main() {
    // Read in the file
    string filename = "day5/input.txt";
    string filestr = file_read(filename);
    vector<string> file = string_split(filestr, '\n');

    /************************ Part 1 ************************/
    vector<pair<long long, long long>> ranges = {};

    // Switch logic for getting ranges or ingredient IDs
    bool get_ranges = true;
    long long total = 0;

    for (int i = 0; i < file.size(); i++) {
        // Reached middle of the file
        if (file[i] == "") {
            get_ranges = false;
            continue;
        }

        // Input ranges as numbers
        if (get_ranges) {
            vector<string> range = string_split(file[i], '-');
            pair<long long, long long> range_conv(stoll(range[0]), stoll(range[1]));
            ranges.push_back(range_conv);
        }
        else {
            long long ingredient_no = stoll(file[i]);

            // Check if ingredient ID is within any range
            for (int r = 0; r < ranges.size(); r++) {
                if (ingredient_no >= ranges[r].first and ingredient_no <= ranges[r].second) {
                    total++;
                    break;
                }
            }
        }
    }

    std::cout << total << '\n';

    /************************ Part 2 ************************/
    total = 0;
    
    loop:
        for (int i = 0; i < ranges.size(); i++) {
            for (int j = 0; j < ranges.size(); j++) {
                if (i != j) {
                    // Check if points in any two ranges intersect 
                    if (
                        (ranges[j].first >= ranges[i].first && ranges[j].first <= ranges[i].second)
                        || (ranges[j].second >= ranges[i].first && ranges[j].second <= ranges[i].second)
                        || (ranges[i].first >= ranges[j].first && ranges[i].first <= ranges[j].second)
                        || (ranges[i].second >= ranges[j].first && ranges[i].second <= ranges[j].second)
                    ) {
                        // Merge any 2 intersecting ranges into one bigger range, removing duplicate indicies
                        ranges[i] = pair(
                                        min({ ranges[i].first, ranges[i].second, ranges[j].first, ranges[j].second }), 
                                        max({ ranges[i].first, ranges[i].second, ranges[j].first, ranges[j].second })
                                    );
                        ranges.erase(ranges.begin() + j);

                        // Since the array values have been messed with, restart the loop
                        goto loop;
                    }
                }
            }
        }

    // Add up the differences between all the combined ranges, adding 1 for inclusivity
    for (int i = 0; i < ranges.size(); i++) {
        total += (ranges[i].second - ranges[i].first) + 1;
    }

    std::cout << total << '\n';

    return 0;
}