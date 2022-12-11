#include <iostream>
#include <limits>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <deque>

using namespace std;

vector<int> stoints(const string& s) {
    vector<int> result;
    regex num_regex("\\d+");
    auto match_begin = sregex_iterator(s.cbegin(), s.cend(), num_regex);
    auto match_end = sregex_iterator();
    for (auto it = match_begin; it != match_end; it++)
        result.emplace_back(stoi(it->str()));
    return result;
}

int main()
{
    //fstream in("input_test");
    fstream in("input");
        
    string result;
    string s;
    vector<deque<char>> stacks;
    while (getline(in, s)) {
        if (s.find("[") != string::npos) {
            for (int i = 0; i < s.length(); i += 4) {
                auto chr = s[i + 1];
                if (chr == ' ') continue;

                auto stackIndex = i/4 + 1;
                if (stackIndex >= stacks.size()) {
                    stacks.resize(stackIndex + 1);
                }
                stacks[stackIndex].push_back(chr);
            }
            
            continue;
        }
        if (s.find("move") == 0)  {
            auto ins = stoints(s);
            auto amount = ins[0];
            auto from = ins[1];
            auto to = ins[2];
            deque<char> temp;
            for (int i = 0; i < amount; i++) {                
                temp.push_front(stacks[from].front());
                stacks[from].pop_front();
            }
            for (int i = 0; i < amount; i++) {                
                auto item = temp.front();
                temp.pop_front();
                stacks[to].push_front(item);
                cout << "moving " << item << " from " << from << " to " << to << endl;
            }
        }
    }        
    for (const auto& st: stacks)
        result += st.front();

    cout << result << endl;
}
