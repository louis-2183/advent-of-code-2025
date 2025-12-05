#include "aocutils.h"
#include <string>
#include <iostream>
#include <cmath>

using namespace std;

// Checks for a paper roll, returning empty space for any position out of bounds
static int safe_check(string grid, int idx, int min, int max) {
    if (idx >= min && idx <= max) {
        if (grid[idx] == '@') {
            return 1;
        }
    }
    return 0;
}

// One iteration of removing rolls, returning the map data and rolls removed
static pair<int,string> remove_iter(string grid, int rowsize, int min_idx, int max_idx) {
    int amount = 0;

    // Copy over the map data for part 2 to alter
    string copy = grid;

    // Rows shifted for looking in 8 directions
    vector<int> yshift = { -1,0,1 };

    for (int i = 0; i < grid.length(); i++) {
        // We are not on a roll of paper - skip any checks
        if (grid[i] != '@') { continue; }

        int rolls_total = 0;

        // Check for @ chars in all 8 directions
        for (int j = 0; j < yshift.size(); j++) {
            if (yshift[j] != 0) {
                rolls_total += safe_check(grid,i + (yshift[j] * rowsize), min_idx, max_idx);
            }
            rolls_total += safe_check(grid,i + (yshift[j] * rowsize) + 1, min_idx, max_idx);
            rolls_total += safe_check(grid,i + (yshift[j] * rowsize) - 1, min_idx, max_idx);
        }

        // If criteria is met, clear the space for the next iteration / increment score
        if (rolls_total < 4) {
            amount++;
            copy[i] = '.';
        }
    }

    return pair(amount,copy);
}

int main() {
    // Read in the file
    string filename = "day4/input.txt";
    string filestr = file_read(filename);

    /************************ Part 1 ************************/
    int answer1 = 0;
    int answer2 = 0;

    int min_idx = 0;
    int max_idx = filestr.length() - 1;
    int rowsize = filestr.find('\n') + 1;
        
    // One iteration of removing rolls
    answer1 = remove_iter(filestr, rowsize, min_idx, max_idx).first;

    /************************ Part 2 ************************/
    int result = 1;

    // Remove rolls until nothing's left
    while (result > 0) {
        pair<int, string> rm_iter = remove_iter(filestr, rowsize, min_idx, max_idx);

        result = rm_iter.first;
        filestr = rm_iter.second;
        answer2 += result;
    }

    std::cout << "Part 1 answer: " << answer1 << '\n';
    std::cout << "Part 2 answer: " << answer2 << '\n';

    return 0;
}
