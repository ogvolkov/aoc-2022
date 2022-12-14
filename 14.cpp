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
    vector<vector<bool>> grid(700, vector<bool>(200));
    int maxy = 0;
    while (getline(in, s)) {
        vector<int> points = stoints(s);
        for (int i = 0; i + 3 < points.size(); i += 2) {
            int x1 = points[i], y1 = points[i+1];
            int x2 = points[i+2], y2 = points[i+3];
            int dx = x2 - x1, dy = y2 - y1;
            if (dx > 0) dx = 1;
            else if (dx < 0) dx = -1;

            if (dy > 0) dy = 1;
            else if (dy < 0) dy = -1;

            for (int x = x1, y = y1; ; x += dx, y += dy) {
                grid[x][y] = true;
                maxy = max(maxy, y);
                if (x == x2 && y == y2) break;
            }
        }
    }
    for (int x = 0; x < grid.size(); x++)
        grid[x][maxy+2] = true;

    int step;
    for (step = 1; ; step++) {
        int x = 500, y = 0;

        for (;;) {
            if (!grid[x][y+1]) {
                y++;
            } else if (!grid[x-1][y+1]) {
                x--;
                y++;
            } else if (!grid[x+1][y+1]) {
                x++;
                y++;
            } else {
                grid[x][y] = true;
                break;
            }
        }

        if (y == 0) break;        
    }

    cout << step << endl;

    return 0;        
}
