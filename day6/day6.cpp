#include "aocutils.h"
#include <string>
#include <iostream>
#include <regex>

using namespace std;

int main() {
    // Read in the file
    string filename = "day6/input.txt";
    string filestr = file_read(filename);
    vector<string> file = string_split(filestr, '\n');

    /************************ Part 1 ************************/
    vector<vector<string>> data{};

    for (int i = 0; i < file.size(); i++) {
        vector<string> parts = string_split(file[i], ' ');
        vector<string> line_feed{};

        for (int j = 0; j < parts.size(); j++) {
            // Determine if the section contains a number or an operator
            string trimmed = regex_replace(parts[j], regex("[^\*\+0-9]"), "");

            // Store it if it does
            if (trimmed.length() > 0) {
                line_feed.push_back(trimmed);
            }
        }

        if (line_feed.size() > 0) {
            data.push_back(line_feed);
        }
    }

    enum oper { ADD, MULTIPLY };
    enum oper oper_type = ADD;
    long long answer = 0;
    long long solution = 0;

    for (int j = 0; j < data[0].size(); j++) {
        long long solution = 0;

        // Loop going upwards to grab the operation first
        for (int i = data.size() - 1; i >= 0; i--) {
            string part = data[i][j];
            
            // Determine operation of the following numbers 
            if (i == data.size() - 1) {
                oper_type = (part == "+") ? ADD : MULTIPLY;
            }
            else {
                // Increment, adding / multipling together
                if (oper_type == MULTIPLY && solution == 0) { solution = 1; }
                solution = (oper_type == ADD) ? solution + stoi(data[i][j]) : solution * stoi(data[i][j]);
            }
        }

        answer += solution;
    }

    std::cout << answer << '\n';

    /************************ Part 2 ************************/
    answer = 0;
    long long vert_num = 0;

    for (int i = 0; i < file[0].length(); i++) {
        vert_num = 0;

        // Check when the first number char moving vertically was found
        int num_index = 0;

        // Loop moving up again, this time character-wise
        for (int j = file.size() - 1; j >= 0; j--) {
            char part = file[j][i];

            // Store the current operation, increment the answer
            if (part == '*' || part == '+') {
                answer += solution;
                solution = 0;
                oper_type = (part == '+') ? ADD : MULTIPLY;
            }

            // If a digit was found, store and power to base 10 depending on the height it was found
            if (int(file[j][i]) >= '0' && int(file[j][i]) <= '9') {
                vert_num += (part - 48) * (pow(10, num_index));
                num_index++;
            }
        }

        // Get one number displayed character-wise vertically
        if (vert_num > 0) {
            if (oper_type == MULTIPLY && solution == 0) { solution = 1; }
            solution = (oper_type == ADD) ? solution + vert_num : solution * vert_num;
        }
    }

    // Increment again since no operator at the end to trigger it
    answer += solution;

    std::cout << answer << '\n';

    return 0;
}