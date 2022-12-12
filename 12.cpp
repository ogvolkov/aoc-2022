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

using namespace std;

int main()
{
    //fstream in("input_test");    
    fstream in("input");
        
    string s;
    vector<string> grid;
    while (getline(in, s)) {
        grid.emplace_back(s);        
    }
    
    int steps = -1;
    deque<pair<int,int>> q;
    vector<vector<bool>> added(grid.size(), vector<bool>(grid[0].length()));
    bool found = false;
    vector<pair<int,int>> dirs {{0,1}, {0,-1}, {1,0}, {-1,0}};

    for (int r = 0; r < grid.size(); r++)
        for (int c = 0; c < grid[r].length(); c++)
            if (grid[r][c] == 'S' || grid[r][c] == 'a') {
                q.emplace_back(r, c);
                added[r][c] = true;
            }

    while (!q.empty() && !found) {
        auto sz = q.size();
        for (int i = 0; i < sz; i++) {
            auto [r, c] = q.front();
            q.pop_front();
         
            if (grid[r][c] == 'E') {
                found = true;
                break;
            }

            auto currentElevation = grid[r][c] != 'S' ? grid[r][c]: 'a';

            for (const auto& dir: dirs) {
                auto nr = r + dir.first;
                auto nc = c + dir.second;
                if (nr < 0 || nr >= grid.size() || nc < 0 || nc >= grid[0].length()) {
                    continue;
                }

                auto nextElevation = grid[nr][nc] == 'S' ? 'a': grid[nr][nc] == 'E' ? 'z': grid[nr][nc];
                if (nextElevation > currentElevation + 1) continue;

                if (added[nr][nc]) continue;

                q.emplace_back(nr, nc);
                added[nr][nc] = true;
            }
        }
        steps++;
    }

    cout << steps << endl;

    return 0;        
}
