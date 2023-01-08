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

void printVec(const vector<int>& nums) {
    for (const auto num: nums) cout << num << " ";
    cout << endl;
}

void printSpace(const vector<pair<int,int>>& nums) {
    for (const auto [idx, num]: nums) cout << num << " ";
    cout << endl;
}

int main()
{
    //fstream in("input_test");    
    fstream in("input");

    string s;
    vector<int> nums;
    while (getline(in, s)) {
        nums.emplace_back(stoi(s));
    } 

    printVec(nums);

    vector<pair<int,int>> space;
    for (int i = 0; i < nums.size(); i++) {
        space.emplace_back(i, nums[i]);
    }
    
    for (int i = 0; i < nums.size(); i++) {
        auto toMove = make_pair(i, nums[i]);
        auto posIt = find(space.begin(), space.end(), toMove); 
        int pos = posIt - space.begin();

        int newPos = pos;
        int d = nums[i];
        while (d != 0) {
            if (d > 0) {
                if (newPos + d < space.size()) {
                    newPos += d;
                    d = 0;
                } else {
                    d -= space.size() - newPos;
                    newPos = 1;                    
                }
            } else {
                if (newPos + d > 0) {
                    newPos += d;
                    d = 0;
                } else {
                    d += newPos;
                    newPos = space.size() - 1;
                }
            }
        }

        //cout << "move " << nums[i] << " newPos=" << newPos << endl;
        
        if (newPos > pos) {
            for (int p = pos; p < newPos; p++)
                space[p] = space[p+1];
            space[newPos] = toMove;
        } else if (newPos < pos) {
            for (int p = pos; p > newPos; p--)
                space[p] = space[p-1];
            space[newPos] = toMove;
        }

        //printSpace(space);
    }

    int zidx;
    for (int i = 0; i < space.size(); i++)
        if (space[i].second == 0) {
            zidx = i;
            break;
        }

    cout << space[(zidx + 1000) % space.size()].second << endl;
    cout << space[(zidx + 2000) % space.size()].second << endl;
    cout << space[(zidx + 3000) % space.size()].second << endl;

    int result = space[(zidx + 1000) % space.size()].second
        + space[(zidx + 2000) % space.size()].second
        + space[(zidx + 3000) % space.size()].second;

    cout << result << endl;

    return 0;
}
