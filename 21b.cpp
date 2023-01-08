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

typedef pair<double, double> Value;

struct Monkey {
    string name;
    Value val;
    char op;
    string monkeyL;
    string monkeyR;
    bool isComputed;
};

Value evalMonkey(const string& name, unordered_map<string, Monkey>& monkeys) {
    if (name == "humn") return {1, 0};

    auto& monkey = monkeys.at(name);    
    if (monkey.isComputed) {
        return monkey.val;
    }

    auto leftVal = evalMonkey(monkey.monkeyL, monkeys);
    auto rightVal = evalMonkey(monkey.monkeyR, monkeys);

    Value val;
    auto op = (name != "root") ? monkey.op : '-';
    switch (op) {
        case '+':
            val = { leftVal.first + rightVal.first, leftVal.second + rightVal.second };
            break;
        case '-':
            val = { leftVal.first - rightVal.first, leftVal.second - rightVal.second };
            break;
        case '*':
            if (leftVal.first * rightVal.first != 0) cout << "WARN non-linear" << endl;
            val = { leftVal.first * rightVal.second + leftVal.second * rightVal.first, leftVal.second * rightVal.second };
            break;
        case '/':
            if (rightVal.first != 0) cout << "WARN divide by non-constant" << endl;
            val = { leftVal.first / rightVal.second, leftVal.second / rightVal.second };
            break;
    }
    monkey.val = val;
    monkey.isComputed = true;

    return val;
}

const regex numMonkeyRegex("(\\w+): (\\d+)");
const regex opMonkeyRegex("(\\w+): (\\w+) (\\+|\\*|-|/) (\\w+)");

Monkey parseMonkey(const string& s) {
    Monkey monkey;
    smatch match;
    if (regex_match(s, match, numMonkeyRegex)) {
        monkey.name = match[1];
        monkey.val = {0, stoi(match[2])};
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

    auto val = evalMonkey("root", monkeys);
    
    cout.precision(17);
    cout << val.first << " *x  " <<  val.second << endl;

    cout << -val.second / val.first << endl;

    return 0;
}
