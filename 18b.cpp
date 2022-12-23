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

const int MAX = 22;

int main()
{
    //fstream in("input_test");    
    fstream in("input");
    
    vector<vector<vector<bool>>> cubes(MAX, vector<vector<bool>>(MAX, vector<bool>(MAX)));
    string s;
    while (getline(in, s)) {
        auto point = stoints(s);
        cubes[point[0]+1][point[1]+1][point[2]+1] = true;
    }   
                
    deque<tuple<int,int,int>> q;
    vector<vector<vector<bool>>> added(MAX+1, vector<vector<bool>>(MAX+1, vector<bool>(MAX+1)));
    for (int x = 0; x < MAX; x++)
        for (int y = 0; y < MAX; y++)
            for (int z = 0; z < MAX; z++)
                if ( (x == 0 || x == MAX-1)
                    || (y == 0 || y == MAX-1)
                    || (z == 0 || z == MAX-1)) {
                        q.emplace_back(x, y, z);
                        added[x][y][z] = true;
                    }

    const vector<tuple<int,int,int>> dirs = {{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}};

    while (!q.empty()) {
        const auto& [x,y,z] = q.front();
        q.pop_front();

        for (const auto& [dx, dy, dz]: dirs) {            
            auto nx = x + dx, ny = y + dy, nz = z + dz;
            if (nx < 0 || nx >= MAX || ny < 0 || ny >= MAX || nz < 0 || nz >= MAX) continue;
            if (added[nx][ny][nz]) continue;
            if (cubes[nx][ny][nz]) continue;

            q.emplace_back(nx, ny, nz);
            added[nx][ny][nz] = true;
        }
    }

    int result = 0;
    for (int x = 0; x < MAX; x++)
        for (int y = 0; y < MAX; y++)
            for (int z = 0; z < MAX; z++)
                if (cubes[x][y][z]) {                    
                    if (added[x-1][y][z]) result++;
                    if (added[x+1][y][z]) result++;
                    if (added[x][y-1][z]) result++;
                    if (added[x][y+1][z]) result++;
                    if (added[x][y][z-1]) result++;
                    if (added[x][y][z+1]) result++;
                }
    
    cout << result << endl;

    return 0;
}
