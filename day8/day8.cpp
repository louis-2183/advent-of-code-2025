#include "aocutils.h"
#include <string>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <set>

using namespace std;

// Single point in space for the problem
class box {
public:
    long x, y, z;
    string id;

    box(long px = 0, long py = 0, long pz = 0) {
        x = px;
        y = py;
        z = pz;
        id = to_string(x) + "," + to_string(y) + "," + to_string(z);
    }

    // Compatability with sets
    bool operator<(const box& rhs) const { 
        return id < rhs.id; 
    }
};

// Connection of two boxes
class connection {
public:
    box box1, box2;
    string id;
    double distance = 0;

    connection(box b1, box b2) {
        box1 = b1;
        box2 = b2;
        distance = hypot(b1.x - b2.x, b1.y - b2.y, b1.z - b2.z); // Euclidean distance
        id = box1.id + "|" + box2.id;
    }

    // Compatability with sets
    bool operator<(const connection& rhs) const { 
        return distance < rhs.distance; 
    }
};

// Sorting function to sort a list of connections by their distance
bool dist_sort(connection i, connection j) { return (i.distance < j.distance); }

int main() {
    // Read in the file
    string filename = "day8/input.txt";
    string filestr = file_read(filename);
    vector<string> file = string_split(filestr, '\n');

    /************************ Parts 1 + 2 ************************/
    vector<box> boxes{};

    // Format the input data into a list of boxes
    for (int i = 0; i < file.size(); i++) {
        vector<string> split = string_split(file[i], ',');
        boxes.push_back(box(stol(split[0]), stol(split[1]), stol(split[2])));
    }

    // Turn these boxes into connections
    set<connection> connections;
    for (int i = 0; i < boxes.size(); i++) {
        for (int j = 0; j < boxes.size(); j++) {
            if (i != j) {
                connections.insert(connection(boxes[i], boxes[j]));
            }
        }
    }

    // List of circuits formed with current connections
    vector<vector<box>> groups{};
    vector<vector<box>> groups_copy{};
    int count_connections = 0;
    int n_connections = 1000; // For part 1
    long long answer1 = 1;
    long long answer2 = 0;

    // Loop through connections in order of distance, merge all available groups
    for (auto& i : connections) {
        // Stop at n_connections required for part 1
        count_connections += 1;

        // List of indicies to merge together
        vector<int> to_union{};
        groups_copy.clear();

        // Find groups that share any box with the latest connection
        for (int j = groups.size() - 1; j >= 0; j--) {
            for (int k = 0; k < groups[j].size(); k++) {
                if (groups[j][k].id == i.box1.id || groups[j][k].id == i.box2.id) {
                    to_union.push_back(j);
                }
            }
        }

        // No mergeable groups currently, make a new one
        if (to_union.size() == 0) {
            groups.push_back(vector({ i.box1, i.box2 }));
        }
        else {
            // Wrap up any box in all shared groups, make them a distinct set
            set<box> distinct;
            distinct.insert(i.box1);
            distinct.insert(i.box2);
            
            for (int j = to_union.size() - 1; j >= 0; j--) {
                for (int k = 0; k < groups[to_union[j]].size(); k++) {
                    distinct.insert(groups[to_union[j]][k]);
                }
            }

            // Copy over anything not in the indicies we have gathered
            for (int j = 0; j < groups.size(); j++) {
                bool found = false;
                for (int k = 0; k < to_union.size(); k++) {
                    if (to_union[k] == j) { found = true; break; }
                }
                if (!found) { groups_copy.push_back(groups[j]); }
            }

            // Put this distinct list back into the copy, effectively merging the shared groups
            vector put_back(distinct.begin(), distinct.end());
            groups_copy.push_back(put_back);

            // First connection to connect all the boxes - part 2
            if (put_back.size() == file.size() && answer2 == 0) {
                answer2 = static_cast<long long>(i.box1.x) * static_cast<long long>(i.box2.x);
                goto answers;
            }

            // Rinse and repeat
            groups = groups_copy;
        }

        // Number of connections reached for part 1
        if (count_connections == n_connections) {
            vector<int> sizes{};

            // Get sizes of all groups
            for (int j = 0; j < groups.size(); j++) {
                sizes.push_back(groups[j].size());
            }

            sort(sizes.begin(), sizes.end());

            // 3 biggest sizes
            for (int j = sizes.size() - 1; j > sizes.size() - 4; j--) {
                answer1 *= sizes[j];
            }
        }
    }

    answers:
        std::cout << answer1 << '\n';
        std::cout << answer2 << '\n';

    return 0;
}