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

struct Monkey {
    string name;
    int64_t val;
    char op;
    string monkeyL;
    string monkeyR;
    bool isComputed;
};

int64_t evalMonkey(const string& name, unordered_map<string, Monkey>& monkeys) {
    auto& monkey = monkeys.at(name);
    if (monkey.isComputed) {
        return monkey.val;
    }

    auto leftVal = evalMonkey(monkey.monkeyL, monkeys);
    auto rightVal = evalMonkey(monkey.monkeyR, monkeys);

    int64_t val;
    switch (monkey.op) {
        case '+':
            val = leftVal + rightVal;
            break;
        case '-':
            val = leftVal - rightVal;
            break;
        case '*':
            val = leftVal * rightVal;
            break;
        case '/':
            val = leftVal / rightVal;
            if (leftVal % rightVal != 0) cout << "WARN not exact" << endl;
            break;
    }
    monkey.val = val;
    monkey.isComputed = true;

    cout << monkey.name << " " << leftVal << " " << monkey.op << " " << rightVal << " = " << val << endl;

    return val;
}

const regex numMonkeyRegex("(\\w+): (\\d+)");
const regex opMonkeyRegex("(\\w+): (\\w+) (\\+|\\*|-|/) (\\w+)");

Monkey parseMonkey(const string& s) {
    Monkey monkey;
    smatch match;
    if (regex_match(s, match, numMonkeyRegex)) {
        monkey.name = match[1];
        monkey.val = stoi(match[2]);
        monkey.isComputed = true;
    } else if (regex_match(s, match, opMonkeyRegex)) {
        monkey.name = match[1];
        monkey.monkeyL = match[2];
        monkey.op = match[3].str().at(0);
        monkey.monkeyR = match[4];
        monkey.isComputed = false;
    }
    return monkey;
}

int main()
{
    //fstream in("input_test");    
    fstream in("input");

    string s;
    unordered_map<string, Monkey> monkeys;
    while (getline(in, s)) {
        auto monkey = parseMonkey(s);
        monkeys.emplace(monkey.name, monkey);
    }

    cout << evalMonkey("root", monkeys) << endl;

    return 0;
}
