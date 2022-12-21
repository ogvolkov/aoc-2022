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
    
    auto shape = shapes.cbegin();
    Board board;
    int highest = 0;
    int wind = 0;

    for (int i = 0; i < 2022; i++) {
        board.resize(highest + 3 + height(*shape), string(7, '.'));
        int sx = 2, sy = highest + 3;
        for (;;) {
            //place(sx, sy, *shape, board, '@');
            //print(board); cout << endl;
            //place(sx, sy, *shape, board, '.');

            //cout << "wind " << winds[wind] << endl;
            auto windDirection = winds[wind] == '>' ? +1: -1;
            if (++wind == winds.length()) wind = 0;

            tryPlace(sx, sy, sx + windDirection, sy, *shape, board);

            //place(sx, sy, *shape, board, '@');
            //print(board); cout << endl;
            //place(sx, sy, *shape, board, '.');

            if (!tryPlace(sx, sy, sx, sy-1, *shape, board)) break;            
        }        
        place(sx, sy, *shape, board, '#');
        //print(board); cout << endl;

        highest = max(highest, sy + height(*shape));
        shape++;
        if (shape == shapes.cend()) shape = shapes.cbegin();
    }
            
    cout << highest << endl;
    return 0;
}
