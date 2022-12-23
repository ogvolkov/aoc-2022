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

using Shape = vector<pair<int,int>>; 
using Board = vector<string>;

const vector<Shape> shapes = {
    {{0,0}, {1,0}, {2,0}, {3, 0}},
    {{1,0}, {0,1}, {1,1}, {2, 1}, {1,2}},
    {{0,0}, {1,0}, {2,0}, {2, 1}, {2,2}},
    {{0,0}, {0,1}, {0,2}, {0, 3}},
    {{0,0}, {1,0}, {0,1}, {1, 1}},
};

int height(const Shape& shape) {
    int my = 0;
    for (const auto [x, y]: shape)
        my = max(my, y);
    return my + 1;
}

void print(const Board& board) {
    for (auto it = board.rbegin(); it != board.rend(); it++) {
        cout << "|" << *it << "|" << endl;
    }
    cout << "+-------+" << endl;
}

bool tryPlace(int& sx, int& sy, int px, int py, const Shape& shape, const Board& board) {
    bool can = true;
    for (const auto [dx,dy]: shape) {
        auto x = px + dx;
        auto y = py + dy;
        if (x < 0 || x >= 7 || y < 0) return false;
        if (board[y][x] != '.') return false;
    }
    if (can) {
        sx = px;
        sy = py;
    }
    return can;
}

void place(int px, int py, const Shape& shape, Board& board, char c) {
    for (const auto [sx,sy]: shape) {
        auto x = px + sx;
        auto y = py + sy;
        board[y][x] = c;
    }
}

int main()
{
    //fstream in("input_test");    
    fstream in("input");
        
    string winds;
    getline(in, winds);
    
    Board board;
    int highest = 0;
    int wind = 0;
    int shapeIdx = 0;

    unordered_map<int, int> highestByNumber;
    unordered_map<string, int> occ;
    int period, delta;
    for (int i = 0; i < 10000; i++) {
        auto& shape = shapes[shapeIdx];
        highestByNumber[i] = highest;

        auto key = (highest > 0 ? board[highest] : "") + to_string(shapeIdx) + ":" + to_string(wind);
        auto prev = occ.find(key);
        if (prev != occ.end()) {
            period = i - prev->second;
            delta = highest - highestByNumber[prev->second];
        }
        occ[key] = i;

        board.resize(highest + 3 + height(shape), string(7, '.'));
        int sx = 2, sy = highest + 3;
        for (;;) {                        
            auto windDirection = winds[wind] == '>' ? +1: -1;
            if (++wind == winds.length()) wind = 0;

            tryPlace(sx, sy, sx + windDirection, sy, shape, board);

            if (!tryPlace(sx, sy, sx, sy-1, shape, board)) break;            
        }        
        place(sx, sy, shape, board, '#');

        highest = max(highest, sy + height(shape));
        if (++shapeIdx == shapes.size()) shapeIdx = 0;       
    }

    long rocks = 1000000000000;
    long result = (rocks / period) * delta + highestByNumber[rocks % period];
            
    cout << result << endl;
    return 0;
}
