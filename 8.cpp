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

int main()
{
    //fstream in("input_test");
    fstream in("input");
        
    string s;
    vector<vector<int>> grid;
    while (getline(in, s)) {
        vector<int> row(s.length());
        for (int i = 0; i < s.length(); i++) {
            row[i] = s[i] - '0';
        }
        grid.emplace_back(row);
    }

    int maxScore = 0;
    for (int r = 0; r < grid.size(); r++)
        for (int c = 0; c < grid[0].size(); c++) {
            int r1, c1;
            int score = 1;

            int count = 1;
            for (r1 = r-1; r1 >= 0 && grid[r1][c] < grid[r][c]; r1--) count++;
            score *= count - (r1 < 0 ? 1: 0);

            count = 1;
            for (r1 = r+1; r1 < grid.size() && grid[r1][c] < grid[r][c]; r1++) count++;
            score *= count - (r1 >= grid.size() ? 1: 0);

            count = 1;
            for (c1 = c-1; c1 >= 0 && grid[r][c1] < grid[r][c]; c1--) count++;
            score *= count -(c1 < 0 ? 1: 0);

            count = 1;
            for (c1 = c+1; c1 < grid[0].size() && grid[r][c1] < grid[r][c]; c1++) count++;
            score *= count - (c1 >= grid[0].size() ? 1: 0);

            maxScore = max(maxScore, score);
        }

    cout << maxScore << endl;
    
    return 0;        
}
