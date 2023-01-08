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
#include <iomanip>
#include <chrono>

using namespace std;

enum Direction  { Right = 0, Down = 1, Left = 2, Up = 3};

int main()
{
    //fstream in("input_test");    
    fstream in("input");

    string s;
    vector<string> map;    
    vector<pair<int,int>> rowBounds;
    vector<pair<int,int>> colBounds;

    int row = 0;
    while (getline(in, s)) {
        if (s == "") break;
        map.emplace_back(s);

        int cMin = numeric_limits<int>::max(), cMax = -1;

        for (int c = 0; c < s.length(); c++) {
            if (s[c] != ' ') {
                cMin = min(cMin, c);
                cMax = max(cMax, c);

                if (colBounds.size() <= c) colBounds.resize(c + 1, {numeric_limits<int>::max(), -1});
                colBounds[c].first = min(colBounds[c].first, row);
                colBounds[c].second = max(colBounds[c].second, row);
            }
        }
        rowBounds.emplace_back(cMin, cMax);

      row++;
    }

    Direction dir = Right;
    int r = 0;
    int c = 0;
    while (c < map.front().length() && map.front()[c] != '.' ) c++;

    while (in) {
        //cout << "r(1)=" << (r+1) << " c(1)=" << (c+1) << " dir=" << dir << endl;

        int steps;
        char rot;
        in >> steps;

        for (int i = 0; i < steps; i++) {
            int nr, nc;
            switch (dir) {
                case Right:
                    nr = r;
                    nc = c + 1;
                    if (nc > rowBounds[r].second) nc = rowBounds[r].first;
                    break;
                case Left:
                    nr = r;
                    nc = c - 1;
                    if (nc < rowBounds[r].first) nc = rowBounds[r].second;
                    break;
                case Down:
                    nr = r + 1;
                    nc = c;
                    if (nr > colBounds[c].second) nr = colBounds[c].first;
                    break;
                case Up:
                    nr = r - 1;
                    nc = c;
                    if (nr < colBounds[c].first) nr = colBounds[c].second;
                    break;
            }

            if (map[nr][nc] == '#') break;
            r = nr;
            c = nc;
        }
        
        in >> rot;
        if (!in) break;

        if (rot == 'R') dir = static_cast<Direction>((dir + 1) % 4);
        else if (rot == 'L')  dir = static_cast<Direction>((dir + 3) % 4);
    }

    cout << 1000 * (r + 1) + 4 * (c + 1 ) + dir << endl;
    
    return 0;
}
