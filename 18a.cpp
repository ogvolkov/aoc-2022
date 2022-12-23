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

const int MAX = 20;

int main()
{
    //fstream in("input_test");    
    fstream in("input");
    
    vector<vector<vector<bool>>> cubes(MAX, vector<vector<bool>>(MAX, vector<bool>(MAX)));
    string s;
    while (getline(in, s)) {
        auto point = stoints(s);
        cubes[point[0]][point[1]][point[2]] = true;
    }
    
    int result = 0;
    for (int x = 0; x < MAX; x++)
        for (int y = 0; y < MAX; y++)
            for (int z = 0; z < MAX; z++)
                if (cubes[x][y][z]) {
                    result += 6;
                    if (x-1 >=0 && cubes[x-1][y][z]) result--;
                    if (x+1 < MAX && cubes[x+1][y][z]) result--;
                    if (y-1 >= 0 && cubes[x][y-1][z]) result--;
                    if (y+1 < MAX && cubes[x][y+1][z]) result--;
                    if (z-1 >= 0 && cubes[x][y][z-1]) result--;
                    if (z+1 < MAX && cubes[x][y][z+1]) result--;
                }
    
    cout << result << endl;

    return 0;
}
