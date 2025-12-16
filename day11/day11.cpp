#include "aocutils.h"
#include <string>
#include <iostream>
#include <regex>
#include <tuple>
#include <unordered_map>
#include <map>

// DFS storing the states for nodes FFT/DAC reached for part 2
long long dfs(unordered_map<string, vector<string>>& paths, map<tuple<string, bool, bool>, long long>& distances, string& current, bool fft, bool dac, bool part2) {
    if (current == "out") {
        // For part 2, only count the path if specific nodes reached
        return part2 ? (int)(fft && dac) : 1;
    }

    // If the distance relating to a specific state has already been found, we don't need to calculate it
    if (distances.find({ current, fft, dac }) != distances.end()) {
        return distances[{current, fft, dac}];
    }

    // Check if nodes FFT/DAC have been reached yet
    if (current == "fft" && !fft) { fft = true; }
    if (current == "dac" && !dac) { dac = true; }

    // Add up all the distances that reach OUT
    long long dist = 0;

    for (auto next : paths[current]) {
        dist += dfs(paths, distances, next, fft, dac, part2);
    }
    
    distances[{current, fft, dac}] = dist;
    
    return dist;
};

int main() {
    string filename = "day11/input.txt";
    string filestr = file_read(filename);
    vector<string> file = string_split(filestr, '\n');

    /************************ Part 1 ************************/
    unordered_map<string, vector<string>> paths;

    // Fetch all path connections
    for (int i = 0; i < file.size(); i++) {
        vector<string> split = string_split(file[i], ' ');
        string key = split[0].substr(0, split[0].length() - 1);
        vector<string> dests(split.begin() + 1, split.end());
        paths[key] = dests;
    }

    map<tuple<string, bool, bool>, long long> distances;
    string current = "you";
    std::cout << dfs(paths, distances, current, false, false, false) << "\n";

    /************************ Part 2 ************************/

    distances.clear();
    current = "svr";
    std::cout << dfs(paths, distances, current, false, false, true) << "\n";
}