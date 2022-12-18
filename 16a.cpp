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

struct Search {
    const int MAX_TIME = 30;
    int best;
    unordered_map<int, int> bestByState;    
    const vector<int>& flow;
    const vector<vector<int>>& graph;
    int maxValveState;

    Search(const vector<int>& flow, const vector<vector<int>>& graph): best(0), flow(flow), graph(graph) {
        maxValveState = 0;
        for (int  i = 0; i < flow.size() && flow[i] > 0; i++)
            maxValveState |= (1 << i);
    }

    void search(int pos, int time, int valveState, int release, int totalRelease) {        
        totalRelease += release;
        if (time >= MAX_TIME || valveState == maxValveState) {            
            totalRelease += (MAX_TIME - time) * release;
            best = max(best, totalRelease);
            return;
        }
        
        auto state = (valveState << 11) | (time << 6) | pos;
        auto bestIt = bestByState.find(state);
        if (bestIt != bestByState.end() && totalRelease <= bestIt->second) {
            return; // prune
        }
        bestByState[state] = totalRelease;

        for (auto next: graph[pos]) {
            auto nextValveState = valveState;
            auto nextRelease = release;

            if (next == pos) {
                if (flow[pos] == 0 || (valveState & (1 << pos)) != 0) continue;

                nextValveState |= (1 << pos);
                nextRelease += flow[pos];
            }

            search(next, time + 1, nextValveState, nextRelease, totalRelease);
        }
    }
};

int main()
{
    fstream in("input_test");    
    //fstream in("input");
        
    string s;
    
    vector<string> valves;
    unordered_map<string, int> flows_string;
    unordered_map<string, vector<string>> adjacent;
    while (getline(in, s)) {
        smatch match;
        if (regex_match(s, match, lineRegex)) {
            auto valve = match[1];
            valves.emplace_back(valve);

            auto flow = stoi(match[2]);
            flows_string[valve] = flow;

            auto to = match[3].str();

            auto match_begin = sregex_iterator(to.cbegin(), to.cend(), toRegex);
            auto match_end = sregex_iterator();
            for (auto it = match_begin; it != match_end; it++) {
                adjacent[valve].emplace_back(it->str());
            }
        }    
    }
    sort(valves.begin(), valves.end(), [&flows_string](auto a, auto b) {
        return flows_string[a] > flows_string[b];
    });

    unordered_map<string, int> valveNo;
    for (int i = 0; i < valves.size(); i++) valveNo[valves[i]] = i;

    vector<int> flow(valves.size());
    for (int i = 0; i < valves.size(); i++)
        flow[i] = flows_string[valves[i]];

    vector<vector<int>> graph(valves.size());
    for (const auto& [vs, adj]: adjacent) {
        auto vNo = valveNo[vs];
        for (const auto& as: adj) {
            auto avNo = valveNo[as];
            graph[vNo].emplace_back(avNo);
        }
        graph[vNo].emplace_back(vNo);
    }

    Search search(flow, graph);
    
    auto start = chrono::system_clock::now();
    search.search(valveNo["AA"], 1, 0, 0, 0);
    auto end = chrono::system_clock::now();

    cout << search.best << endl;
    cout << "in time " << chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << "ms" << endl;
    return 0;        
}
