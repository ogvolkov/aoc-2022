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
        
    string s;
    while (getline(in, s)) {
        cout << s << endl;
        unordered_map<char, int> seen;
        auto start = 0;
        for (int i = 0; i < s.length(); i++) {
            if (i - start == 14) {
                cout << i << endl;
                break;
            }

            auto prev = seen.find(s[i]);
            if (prev != seen.end()) {
                auto bk = start;
                start = max(start, prev->second + 1);
                if (start != bk) cout << " restart at " << start << " cause " << s[i] << " found at " << prev->second << endl;
            }
            seen[s[i]] = i;            
        }
    }        
}
