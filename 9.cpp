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

using namespace std;

void moveHead(pair<int,int>& h, char where) {
    switch (where) {
        case 'U':
            h.first++;
            break;
        case 'D':
            h.first--;
            break;
        case 'L':
            h.second--;
            break;
        case 'R':
            h.second++;
            break;    
    }
}

void moveKnot(const pair<int,int>& h, pair<int,int>& t) {
    auto d = make_pair(t.first - h.first, t.second - h.second);
    if (abs(d.first) < 2 && abs(d.second) < 2) return;
    
    if (d.first < 0) d.first++;
    else if (d.first > 0) d.first--;

    if (d.second < 0) d.second++;
    else if (d.second > 0) d.second--;

    t = make_pair(h.first + d.first, h.second + d.second);
}

int main()
{
    //fstream in("input_test");    
    fstream in("input");
        
    string s;
    set<pair<int,int>> visited;
    vector<pair<int,int>> knots(10);
    visited.emplace(knots.back());
    while (getline(in, s)) {
        auto moves = stoi(s.substr(2));
        for (int i = 0; i < moves; i++){
            moveHead(knots[0], s[0]);

            for (int i = 1; i < knots.size(); i++)
                moveKnot(knots[i-1], knots[i]);

            for (int r = 20; r >= -20; r--) {
                for (int c = -20; c < 20; c++) {
                    auto p = make_pair(r, c);
                    auto filled = false;
                    for (int i = 0; i < knots.size() && !filled; i++)
                        if(knots[i] == p) {
                            if (i == 0) cout << 'H';
                            else cout << i;
                            filled = true;
                            break;
                        }
                    
                    if (!filled) {
                        if (r == 0 && c ==0) cout << "s";
                        else cout << ".";
                    }
                }   
                cout << endl;                 
            }
            cout << endl;

            visited.emplace(knots.back());
        }
    }

    
    for (int r = 20; r >= -20; r--) {
        for (int c = -20; c < 20; c++) {
            if (r == 0 && c == 0) cout << "s";
            else if (visited.count(make_pair(r, c)) > 0) cout << "x";
            else cout << ".";
        }   
        cout << endl;                 
    }
    cout << endl;


    cout << visited.size() << endl;
    
    return 0;        
}
