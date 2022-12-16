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

regex lineRegex("Valve (\\w+) has flow rate=(\\d+); tunnels? leads? to valves? (.+)");
regex toRegex("(\\w+)");

struct Setup {
    int nz;
    int no;
    unordered_map<string, int> mp;
    unordered_map<string, int> stateful;
    unordered_map<string, int> flows;
    unordered_map<string, vector<string>> graph;

    Setup(): nz(0), no(0) {}
};

struct Search {
    int best;
    unordered_map<int, int> bestByState;
    Setup& setup;

    Search(Setup& setup): best(0), setup(setup) {}

    void dfs(const string& valve, int time, int valveState, int release, int totalRelease) {        
        totalRelease += release;
        if (time >= 30) {
            best = max(best, totalRelease);
            return;
        }
        
        auto state = (valveState << 11) | (time << 6) | setup.mp[valve];
        auto bestIt = bestByState.find(state);
        if (bestIt != bestByState.end() && totalRelease <= bestIt->second) {
            return; // prune
        }
        bestByState[state] = totalRelease;

        // try open
        const auto statefulIt = setup.stateful.find(valve);
        if (statefulIt != setup.stateful.end()) {
            auto valveNo = statefulIt->second;

            if ((valveState & (1 << valveNo)) == 0) {
                auto nextValveState = valveState | (1 << valveNo);
                auto nextRelease = release + setup.flows.at(valve);

                dfs(valve, time + 1, nextValveState, nextRelease, totalRelease);
            }
        }

        // move
        for (const auto& nextValve: setup.graph.at(valve)) {
            dfs(nextValve, time + 1, valveState, release, totalRelease);
        }
    }
};

int main()
{
    //fstream in("input_test");    
    fstream in("input");
        
    string s;
    
    Setup setup;
    while (getline(in, s)) {
        smatch match;
        if (regex_match(s, match, lineRegex)) {
            auto valve = match[1];
            auto flow = stoi(match[2]);

            setup.mp[valve] = setup.no++;
            if (flow > 0) {
                setup.stateful[valve] = setup.nz++;
            }

            setup.flows[valve] = flow;

            auto to = match[3].str();

            auto match_begin = sregex_iterator(to.cbegin(), to.cend(), toRegex);
            auto match_end = sregex_iterator();
            for (auto it = match_begin; it != match_end; it++) {
                setup.graph[valve].emplace_back(it->str());
            }
        }    
    }
    Search search(setup);
    search.dfs("AA", 1, 0, 0, 0);
    cout << search.best << endl;
    return 0;        
}
