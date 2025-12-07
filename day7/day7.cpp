#include "aocutils.h"
#include <string>
#include <iostream>
#include <unordered_map>

using namespace std;

// Custom object for unique beam position tracking / timeline counting
class beamcounter {
public:
    unordered_map<int, long long> counter;

    // Add a beam position or increment timeline count
    void add_increment(const pair<int, long long>& item) {
        counter[item.first] += item.second;
    }

    void clear() {
        counter.clear();
    }
};

int main() {
    // Read in the file
    string filename = "day7/input.txt";
    string filestr = file_read(filename);
    vector<string> file = string_split(filestr, '\n');

    /************************ Parts 1 + 2 ************************/
    beamcounter beam_positions;
    beamcounter copied_positions;

    // Initial beam position, start at only 1 timeline
    beam_positions.add_increment(pair(file[0].find('S'), 1));

    int answer1 = 0;
    long long answer2 = 0;

    for (int i = 0; i < file.size(); i++) {
        copied_positions.clear();

        // For each UNIQUE beam position, split it (adding counts of timelines)
        for (auto& j : beam_positions.counter) {
            // Split a beam
            if (file[i][j.first] == '^') {
                copied_positions.add_increment(pair(j.first - 1, j.second));
                copied_positions.add_increment(pair(j.first + 1, j.second));
                answer1++;
            }
            // Carry beam forward
            else {
                copied_positions.add_increment(pair(j.first, j.second));
            }
        }

        beam_positions = copied_positions;
    }

    // Add up all the unique beam positions' timeline counts
    for (auto& k : beam_positions.counter) {
        answer2 += k.second;
    }

    std::cout << answer1 << '\n';
    std::cout << answer2 << '\n';

    return 0;
}