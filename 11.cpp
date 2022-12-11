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

using namespace std;

struct Monkey {
    int index;
    deque<long> things;
    string operand1;
    string operation;
    string operand2;
    int divisor;
    unordered_map<bool, int> nextMonkey;
    long inspectionCount;
};

int main()
{
    //fstream in("input_test");    
    fstream in("input");
        
    regex monkeyRegex("Monkey (\\d+):");
    regex itemsRegex("  Starting items: (\\d+\\D*)*");
    regex operationRegex("  Operation: new = (\\w+) (.) (\\w+)");
    regex testRegex("  Test: divisible by (\\d+)");
    regex branchRegex("    If (\\w+): throw to monkey (\\d+)");

    string s;
    int monkey;    
    vector<Monkey> monkeys;
    int modulo = 1;
    while (getline(in, s)) {
        smatch match;
        if (regex_match(s, match, monkeyRegex)) {
            monkey = stoi(match[1]);
            monkeys.resize(monkey + 1);
            monkeys[monkey].index = monkey;
            cout << "monkey " << monkey << endl;
        }
        if (regex_match(s, match, itemsRegex)) {            
            regex num_regex("\\d+");
            auto match_begin = sregex_iterator(s.cbegin(), s.cend(), num_regex);
            auto match_end = sregex_iterator();
            for (auto it = match_begin; it != match_end; it++) {
                auto item = stoi(it->str());
                monkeys[monkey].things.emplace_back(item);
                cout << "item " << item << endl;
            }
        }
        if (regex_match(s, match, operationRegex)) {
            monkeys[monkey].operand1 = match[1];
            monkeys[monkey].operation = match[2];
            monkeys[monkey].operand2 = match[3];            
            cout << "operation " << match[1] << " " << match[2] << " " << match[3] << endl;            
        }
        if (regex_match(s, match, testRegex)) {
             monkeys[monkey].divisor = stoi(match[1]);
             modulo *= monkeys[monkey].divisor;
             cout << "divisor " << monkeys[monkey].divisor << endl;
        }
        if (regex_match(s, match, branchRegex)) {
            cout << " branch " << match[1] << " " << match[2] << endl;
            monkeys[monkey].nextMonkey[match[1] == "true"] = stoi(match[2]);
        }
    }

    cout << "modulo = " << modulo << endl;

    for (int rnd = 1; rnd <= 10000; rnd++) {
        cout << "Round " << rnd << endl;
        for (auto& monkey: monkeys) {
            // cout << "Monkey " << monkey.index << endl;
            while (!monkey.things.empty()) {
                auto thing = monkey.things.front();
                monkey.things.pop_front();

                // cout << "  Monkey inspects an item with a worry level of " << thing << endl;
                
                monkey.inspectionCount++;
                
                auto op1 = monkey.operand1 == "old" ? thing: stoi(monkey.operand1);
                auto op2 = monkey.operand2 == "old" ? thing: stoi(monkey.operand2);
                if (monkey.operation == "*") thing = op1 * op2;
                else thing = op1 + op2;

                thing %= modulo;

                // cout << "    Worry level to " << thing << endl;

                auto to = monkey.nextMonkey[thing % monkey.divisor == 0];
                // cout << "    Item with worry level " << thing << " is thrown to monkey " << to << endl;
                monkeys[to].things.emplace_back(thing);
            }
        }
    }

    sort(monkeys.begin(), monkeys.end(), [](auto a, auto b) {
        return a.inspectionCount > b.inspectionCount;
    });
    
    cout << monkeys[0].inspectionCount << endl;
    cout << monkeys[1].inspectionCount << endl;

    cout << monkeys[0].inspectionCount * monkeys[1].inspectionCount << endl;

    return 0;        
}
