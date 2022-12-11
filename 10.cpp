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

int main()
{
    //fstream in("input_test");    
    fstream in("input");
        
    string s;
    deque<pair<string,int>> program;
    int result = 0, x = 1, lastStart = 0;    
    for (int i = 1; i <= 240; i++) {        
        int pos = (i-1) % 40;
        if (pos >= x-1 && pos <= x+1) cout << "#";
        else cout << ".";
        if (i % 40 == 0) cout << endl;

        if (getline(in, s)) {            
            if (s.find("addx") == 0) program.emplace_back("addx", stoi(s.substr(5)));
            else program.emplace_back("noop", 0);
        }        

        if (!program.empty()) {
            bool finished = true;
            auto currentInstruction = program.front();
            if (currentInstruction.first == "addx") {
                finished = i == lastStart + 2;
                if (finished) x += currentInstruction.second;                
            }

            if (finished) {
                program.pop_front();
                lastStart = i;
            }
        }
    }
    
    return 0;        
}
