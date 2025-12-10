#include "aocutils.h"
#include <string>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <set>
#include <unordered_map>

using namespace std;

// Rectangular area of the grid, defined with 2 points
class rect {
public:
    int x1, x2, y1, y2;
    long long area;

    rect(int px1 = 0, int py1 = 0, int px2 = 0, int py2 = 0) {
        x1 = min(px1,px2);
        y1 = min(py1,py2);
        x2 = max(px1,px2);
        y2 = max(py1,py2);
        area = (static_cast<long long>(abs(x2 - x1)) + 1) * (static_cast<long long>(abs(y2 - y1)) + 1);
    }

    // Check to see if a line intersects with the area inside the borders of the rectangle
    bool inner_crossover(int lx1, int ly1, int lx2, int ly2) const {
        int lxm1 = min(lx1, lx2);
        int lxm2 = max(lx1, lx2);
        int lym1 = min(ly1, ly2);
        int lym2 = max(ly1, ly2);

        for (int x = min(lx1, lx2); x <= lxm2; x++) {
            for (int y = lym1; y <= lym2; y++) {
                if (x >= x1 + 1 && x <= x2 - 1 && y >= y1 + 1 && y <= y2 - 1) {
                    return true; // Intersecting
                }
            }
        }

        return false;
    }

    // For sorting with (reversed)
    bool operator<(const rect& rhs) const {
        return area > rhs.area;
    }
};

int main() {
    // Read in the file
    string filename = "day9/input.txt";
    string filestr = file_read(filename);
    vector<string> file = string_split(filestr, '\n');

    /************************ Part 1 ************************/
    int max_area = 0;

    // List of rectangles for later
    vector<rect> rects{};

    for (int i = 0; i < file.size(); i++) {
        for (int j = 0; j < file.size(); j++) {
            if (j != i) {
                // Compare every pairwise combination, setting a record for the multiplied area
                vector<string> split_p1 = string_split(file[i], ',');
                vector<string> split_p2 = string_split(file[j], ',');

                rects.push_back(rect(stoi(split_p1[0]), stoi(split_p1[1]), stoi(split_p2[0]), stoi(split_p2[1])));
            }
        }
    }

    sort(rects.begin(), rects.end());
    std::cout << rects[0].area << '\n';

    /************************ Part 2 ************************/
    set<pair<pair<int, int>,pair<int,int>>> borders;

    // Loop through and append pairs of sequential points as borders
    file.push_back(file[0]);

    for (int i = 0; i < file.size() - 1; i++) {
        int next = i + 1;

        vector<string> split_p1 = string_split(file[i], ',');
        vector<string> split_p2 = string_split(file[next], ',');

        borders.insert(pair(pair(stol(split_p1[0]), stol(split_p1[1])),pair(stol(split_p2[0]), stol(split_p2[1]))));
    }

    // Since checking every single space on a 2D grid would not finish in this lifetime (and I failed miserably at heuristics), the new approach:
    // Check every rectangle biggest to smallest to see if any border lines intersect with it - and they are not intersecting only its own border.
    // If no intersection happens with any borders, the area found will be inside ALL borders.
    for (int i = 0; i < rects.size(); i++) {
        bool crossover = false;

        // Check for an intersection with any border
        for (auto &j : borders) {
            if (rects[i].inner_crossover(j.first.first, j.first.second, j.second.first, j.second.second)) {
                crossover = true;
                break;
            };
        }

        if (!crossover) {
            std::cout << rects[i].area << '\n';
            break;
        }
    }
}