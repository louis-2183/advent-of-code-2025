#include "aocutils.h"
#include <string>
#include <iostream>
#include <regex>
#include <set>
#include <unordered_map>
#include <map>
#include <climits>

// Takes a string for a button and provides its binary value
int from_binary(string s) {
    int out = 0;

    for (int i = 0; i < s.length(); i++) {
        out += (s[i] == '#') ? (1LL << i) : 0;
    }

    return out;
}

// Holds the button joltage value and it's binary value for lights
class button {
public:
    uint16_t bin_value = 0;  
    vector<int> values;
    string txt;

    button(string s) {
        txt = "(" + s + ")";
        vector<string> split = string_split(s, ',');
        
        for (int i = 0; i < split.size(); i++) {
            int value = stoi(split[i]);
            values.push_back(value);
            bin_value += (1LL << value);
        }
    }

    bool operator<(const button& rhs) const { 
        return bin_value < rhs.bin_value; 
    }

    bool operator==(const button& rhs) const {
        return bin_value == rhs.bin_value; 
    }
};

// Container for joltages, for addressing part 2
class joltage {
public:
    vector<int> values;
    int depth = 0;
    long long presses = 0;  

    joltage(vector<int> v) {
        values = v;
    }

    int to_light() {
        int out = 0;

        for (int i = 0; i < values.size(); i++) {
            if (values[i] % 2 != 0) {
                out += (1LL << i);
            }
        }

        return out;
    }

    void subtract(joltage j) {
        for (int i = 0; i < values.size(); i++) {
            values[i] -= j.values[i];
        }
    }

    bool is_above_zero() {
        for (int i : values) if (i < 0) return false;
        return true;
    }

    bool is_zero() {
        for (int i : values) if (i != 0) return false;
        return true;
    }

    bool is_even() {
        for (int i : values) if (i % 2 != 0) return false;
        return true;
    }

    void halve() {
        for (int i = 0; i < values.size(); i++) {
            values[i] /= 2;
        }
        depth++; // Since the joltage has been halved, every other press to solve it will be * 2
    }

    string get_text() {
        string out;

        for (int i = 0; i < values.size(); i++) {
            if (i > 0) out += ",";
            out += to_string(values[i]);
        }

        return "{" + out + "}";
    }

    bool operator<(const joltage& rhs) const { return values < rhs.values; }
};

// History of button presses
class buttonhistory {
public:
    vector<button> history;
    uint16_t bin_value = 0;

    buttonhistory(button b) {
        bin_value = b.bin_value;
        history.push_back(b);
    }

    // Add to history while updating overall XOR value
    void append(button b) {
        bin_value ^= b.bin_value;
        history.push_back(b);
        sort(history.begin(), history.end());
    }

    size_t size() { return history.size(); }
    
    // Returns a joltage formatted with the same length as the target
    joltage to_joltage(int maxlen) {
        vector<int> out(maxlen, 0);
        
        for (auto button : history) {
            for (int i = 0; i < button.values.size(); i++) {
                out[button.values[i]]++;
            }
        }

        return joltage(out);
    }

    bool operator<(const buttonhistory& rhs) const { return history < rhs.history; }
};

// Part 1 - perform a BFS
int bfs(set<buttonhistory> curr, set<button> buttons, int target, int solved) {
    int solved_updated = solved;
    set<buttonhistory> updated;

    for (auto i : curr) {
        if (i.size() <= buttons.size()) {
            // Found a matching button press combination
            if (i.bin_value == target) {
                if (i.size() < solved_updated) solved_updated = i.size();
            }
            else {
                for (auto j : buttons) {
                    // Add a next button if there are no repeats, every button should only be pressed 0 or 1 times
                    if (find(i.history.begin(), i.history.end(), j) == i.history.end()) {
                        buttonhistory temp = i;
                        temp.append(j);

                        if (temp.history.size() <= buttons.size()) {
                            updated.insert(temp);
                        }
                    }
                }
            }
        }
    }

    if (updated.size() > 0) {
        return bfs(updated, buttons, target, solved_updated);
    }

    return solved_updated;
}

// For part 2
// Gets all possible combinations of pressing a button 0 or 1 times
// Adjusted for small inaccuracies - on the first press, check if any button has been pressed 0-4 consecutive times??
set<pair<joltage, int>> find_all_combs(set<button> buttons, joltage target, bool allow_multi_press = false) {
    vector<button> button_vec(buttons.begin(), buttons.end());
    set<pair<joltage, int>> solutions;

    int n_buttons = button_vec.size();
    int target_light = target.to_light();

    // Increasing this to 4 still explodes the amount of searches (although gets the right answer)...
    int presses = allow_multi_press ? 4 : 2;
    int n_combs = 1;

    for (int i = 0; i < n_buttons; i++) {
        n_combs *= presses;
    }

    for (int state = 0; state < n_combs; state++) {
        vector<int> resulting_joltage(target.values.size(), 0);
        uint16_t xor_sum = 0;
        int count = 0;
        int temp = state;

        for (int i = 0; i < n_buttons; ++i) {
            // Times the button was pressed in this state
            int times = temp % presses;
            temp /= presses;
            count += times;

            // If the button was pressed an odd amount, flip the bits
            if (times % 2 == 1) {
                xor_sum ^= button_vec[i].bin_value;
            }

            // Count effect: always add full times
            for (int light : button_vec[i].values) {
                resulting_joltage[light] += times;
            }
        }

        // If the buttons follow the light pattern needed, record the effect on overall joltage
        if (xor_sum == target_light) {
            joltage remaining = target;
            joltage new_joltage(resulting_joltage);

            remaining.subtract(new_joltage);
            
            if (remaining.is_above_zero()) {
                solutions.emplace(new_joltage, count);
            }
        }
    }

    return solutions;
}

// Store any combination results for lights on a given machine to re-use again
// Speeds things up to the point I can actually get a result
class cache {
public:
    using key = int;
    using map = unordered_map<key, set<pair<joltage, int>>>;
    map m;

    // When the cache is first initialised, for the first grab of combinations, try multiple presses of the same button
    bool allow_multi_press = true;

    set<pair<joltage, int>> memo_combs(set<buttonhistory> curr, set<button> buttons, joltage target, set<pair<joltage, int>> solved) {
        key k = target.to_light();
        auto it = m.find(k);

        // Do the heavy lifting if needed
        if (it == m.end()) {
            auto result = find_all_combs(buttons, target, allow_multi_press);
            
            // No multi presses any more, dividing and conquering should account for the rest
            allow_multi_press = false;
            m[k] = result;
            return result;
        }

        return it->second;
    }
};

// For part 2
// When a joltage has at least 1 odd number, find all minimum presses for part 1
// This will reduce the joltage to be fully even
// Divide by 2 and repeat - if it is zero we can store the amount of presses and get the min
long long divide_and_conquer(set<pair<joltage, long long>> current_joltages, cache& mem, set<buttonhistory> curr, set<button> buttons, long long result) {
    set<pair<joltage, long long>> updated_joltages;
    long long result_updated = result;

    for (auto [current, presses] : current_joltages) {
        // Stop looking over the smallest record 
        if (presses >= result_updated) continue; 

        // Path of buttons to press to get the target joltage
        if (current.is_zero()) {
            result_updated = min(result_updated, presses);
            continue;
        }

        // Invalid path to target joltage
        if (!current.is_above_zero()) continue;

        // Even joltage - divide by two until it's odd again
        if (current.is_even()) {
            joltage temp = current;

            while (temp.is_even()) {
                temp.halve();
            }
            
            updated_joltages.emplace(temp, presses);
        }
        else {
            // Get all ways to reduce the odd joltage to a fully even amount
            set<pair<joltage, int>> solutions = mem.memo_combs(curr, buttons, current, set<pair<joltage, int>>{});

            for (auto [new_joltage, count] : solutions) {
                joltage temp = current;
                temp.subtract(new_joltage);

                // For every halving of the joltage, 2 * button presses will be needed down the line
                long long new_presses = presses + (count * (1LL << temp.depth)); 
                
                if (new_presses < result_updated) {
                    // Path of buttons to press to get the target joltage
                    if (temp.is_zero()) {
                        result_updated = min(result_updated, new_presses);
                    }
                    else if (temp.is_above_zero()) {
                        // Even joltage for next iteration
                        updated_joltages.emplace(temp, new_presses);
                    }
                }
            }
        }
    }

    if (updated_joltages.size() > 0) {
        return divide_and_conquer(updated_joltages, mem, curr, buttons, result_updated);
    }

    return result_updated;
}

int main() {
    string filename = "day10/input.txt";
    string filestr = file_read(filename);
    vector<string> file = string_split(filestr, '\n');

    long long answer1 = 0;
    long long answer2 = 0;

    /************************ Part 1 ************************/
    for (int i = 0; i < file.size(); i++) {
        string formatted = regex_replace(file[i], regex(R"([^\s#.,0-9])"), "");
        vector<string> split = string_split(formatted, ' ');

        // Record lights
        int light_input = from_binary(split[0]);

        // Record buttons
        set<buttonhistory> button_hist;
        set<button> buttons;

        for (int j = 1; j < split.size() - 1; j++) {
            button_hist.insert(buttonhistory(button(split[j])));
            buttons.insert(button(split[j]));
        }

        int result = bfs(button_hist, buttons, light_input, 1000000);
        answer1 += result;
    }
    std::cout << "Part 1 answer: " << answer1 << '\n';

    /************************ Part 2 ************************/
    // Separating this due to the time it takes to get a right answer due to the sheer number of searches

    for (int i = 0; i < file.size(); i++) {
        string formatted = regex_replace(file[i], regex(R"([^\s#.,0-9])"), "");
        vector<string> split = string_split(formatted, ' ');

        // Record lights
        int target_light = from_binary(split[0]);
        
        // Record buttons
        set<buttonhistory> button_hist;
        set<button> buttons;

        for (int j = 1; j < split.size() - 1; ++j) {
            button b(split[j]);
            button_hist.emplace(b);
            buttons.emplace(b);
        }

        // Record joltage
        vector<int> joltage_input;
        split = string_split(split[split.size() - 1], ',');

        for (int j = 0; j < split.size(); j++) {
            joltage_input.push_back(stoi(split[j]));
        }
        joltage target_joltage(joltage_input);

        // Start recording answers for this line
        cache c;

        set<pair<joltage, long long>> solved_joltages{ {target_joltage, 0} };
        long long result = divide_and_conquer(solved_joltages, c, button_hist, buttons, 1000000);
        
        //std::cout << result << '\n';
        answer2 += result;
    }
    std::cout << "Part 2 answer: " << answer2 << '\n';
}