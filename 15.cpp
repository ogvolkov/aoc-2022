#include <iostream>
#include <limits>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <deque>
#include <cmath>
#include <cassert>

using namespace std;

vector<int> stoints(const string& s) {
    vector<int> result;
    regex num_regex("-?\\d+");
    auto match_begin = sregex_iterator(s.cbegin(), s.cend(), num_regex);
    auto match_end = sregex_iterator();
    for (auto it = match_begin; it != match_end; it++)
        result.emplace_back(stoi(it->str()));
    return result;
}

vector<pair<int,int>> buildIntervals(const vector<vector<int>>& allPoints, int targety) {
    vector<pair<int,int>> intervals;

    for (const auto& points: allPoints) {
        auto sx = points[0];
        auto sy = points[1];
        auto bx = points[2];
        auto by = points[3];

        auto dist = abs(bx - sx) + abs(by - sy);
        auto dx = dist - abs(sy - targety);
        if (dx >= 0) intervals.emplace_back(sx - dx, sx + dx);
    }

    sort(intervals.begin(), intervals.end());

    return intervals;
}

int main()
{
    //int mx = 20, my = 20;
    //fstream in("input_test");    
    int mx = 4000000, my = 4000000;
    fstream in("input");
        
    string s;    
    vector<vector<int>> allPoints;
    while (getline(in, s)) {
        allPoints.emplace_back(stoints(s));        
    }
    
    for (int y = 0; y <= my; y++) {
        auto intervals = buildIntervals(allPoints, y);
        int expectedStart = 0;
        int x = 0;
        bool found = false;
        for (const auto [from, to]: intervals) {
            if (to < expectedStart) continue;

            if (from > expectedStart) {
                x = expectedStart;                
                found = true;
                break;
            }            
            expectedStart = to + 1;
            if (expectedStart > mx) break;
        }        
        if (!found && expectedStart <= mx) {
            x = expectedStart;
            found = true;
        }

        if (found) {
            cout << x << " " << y << endl;
            cout << 4000000 * (long)x + y << endl;
            break;
        }
    }    

    return 0;        
}
