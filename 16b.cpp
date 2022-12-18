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
#include <chrono>

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

    void dfs(const string& posA, const string& posB, int time, int valveState, int release, int totalRelease) {        
        totalRelease += release;
                
        if (time >= 26) {
            best = max(best, totalRelease);
            return;
        }
        
        auto state = (valveState << 17) | (time << 12) | (setup.mp[posA] << 6) | setup.mp[posB];
        auto bestIt = bestByState.find(state);
        if (bestIt != bestByState.end() && totalRelease <= bestIt->second) {
            return; // prune
        }
        bestByState[state] = totalRelease;

        for (const auto& nextPosA: setup.graph.at(posA))
            for (const auto& nextPosB: setup.graph.at(posB)) {
                auto nextValveState = valveState;
                auto nextRelease = release;
                if (nextPosA == posA) {
                    const auto statefulIt = setup.stateful.find(posA);
                    if (statefulIt != setup.stateful.end()) {
                        auto valveNo = statefulIt->second;

                        if ((nextValveState & (1 << valveNo)) == 0) {
                            nextValveState |= (1 << valveNo);
                            nextRelease += setup.flows.at(posA);                        
                        }
                    }
                }
                if (nextPosB == posB) {
                    const auto statefulIt = setup.stateful.find(posB);
                    if (statefulIt != setup.stateful.end()) {
                        auto valveNo = statefulIt->second;

                        if ((nextValveState & (1 << valveNo)) == 0) {
                            nextValveState |= (1 << valveNo);
                            nextRelease += setup.flows.at(posB);                        
                        }
                    }
                }                
                dfs(nextPosA, nextPosB, time + 1, nextValveState, nextRelease, totalRelease);
            }        
    }
};

int main()
{
    fstream in("input_test");    
    //fstream in("input");
        
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
            setup.graph[valve].emplace_back(valve);
        }    
    }

    Search search(setup);
    auto start = chrono::system_clock::now();
    search.dfs("AA", "AA", 1, 0, 0, 0);
    auto end = chrono::system_clock::now();
    cout << search.best << endl;

    cout << "in time " << chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << "ms" << endl;
    return 0;        
}
