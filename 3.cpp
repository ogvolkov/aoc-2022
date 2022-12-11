#include <iostream>
#include <limits>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    //fstream in("input_test");
    fstream in("input");
    
    int score = 0; 
    while (in.peek() != EOF) {
        vector<string> s(3);
        for (int i = 0; i < 3; i++)
            in >> s[i];
        
        vector<unordered_map<char, int>> count(3);
        for (int i = 0; i < 3; i++)
            for (char c: s[i]) count[i][c]++;
        
        char dup;
        for (const auto [c, v]: count[0]) {
            bool isDup = true;         
            for (int i = 1; i < 3; i++)
                if (count[i][c] == 0) isDup = false;

            if (isDup) dup = c;
        }

        cout << dup << endl;

        score += (dup >= 'a' && dup <= 'z') ?dup - 'a' + 1: dup - 'A' + 27;
    }        
    cout << score << endl;
}
