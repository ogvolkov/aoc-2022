#include <iostream>
#include <limits>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <vector>
#include <string>
#include <regex>

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
        
    int score = 0;
    string s;
    while (in >> s) {
        auto nums  = stoints(s);
        // [n0..n1]  [n2..n3]
        if (nums[0] <= nums[3] && nums[1] >= nums[2]) {
            //cout << nums[0] << " " << nums[1] << ", " << nums[2] << " " << nums[3] << endl;
            score++;
        }
    }        
    cout << score << endl;
}
