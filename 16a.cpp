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
    int nz;
    int best;
    unordered_map<int, int> bestByState;    
    const vector<int>& flow;
    const vector<vector<int>>& graph;
    const vector<string>& valves;
    int maxValveState;

    Search(int nz, const vector<int>& flow, const vector<vector<int>>& graph, const vector<string>& valves)
        : nz(nz), best(0), flow(flow), graph(graph), valves(valves) {
        maxValveState = (1 << nz) - 1;        
    }

    void search(int pos, int time, int valveState, int release, int totalRelease) {       
        if (time >= MAX_TIME || valveState == maxValveState) {    
            totalRelease += (MAX_TIME - time + 1) * release;
            best = max(best, totalRelease);
            return;
        }
        
        auto state = (valveState << 11) | (time << 6) | pos;
        auto bestIt = bestByState.find(state);
        if (bestIt != bestByState.end() && totalRelease <= bestIt->second) {
            return; // prune
        }
        bestByState[state] = totalRelease;

        // turn valve if possible
        if ((valveState & (1 << pos)) == 0) {
            search(pos, time + 1, valveState | (1 << pos), release + flow[pos], totalRelease + release);                
            return;
        }

        // move
        for (int next = 0; next < nz; next++)
            if (next != pos) {
                auto nextTime = min(MAX_TIME, time + graph[pos][next]);
                search(next, nextTime, valveState, release, totalRelease + release * (nextTime - time));
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
    for (int i = 0; i < valves.size(); i++)
        valveNo[valves[i]] = i;
        
    vector<int> flow(valves.size());
    for (int i = 0; i < valves.size(); i++)
        flow[i] = flows_string[valves[i]];
    
    vector<vector<int>> dist (valves.size(), vector<int>(valves.size(), 100));
    for (int i = 0; i < valves.size(); i++) dist[i][i] = 0;
    
    for (const auto& [vs, adj]: adjacent) {
        auto vNo = valveNo[vs];
        for (const auto& as: adj) {
            auto avNo = valveNo[as];
            dist[vNo][avNo] = 1;
            dist[avNo][vNo] = 1;            
        }        
    }

    for (int k = 0; k < valves.size(); k++)
        for (int i = 0; i < valves.size(); i++)
            for (int j = 0; j < valves.size(); j++)
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);

    int nz = 0;
    for (auto f: flow) if (f > 0) nz++;

    Search search(nz, flow, dist, valves);
    
    auto from = valveNo["AA"];
    auto start = chrono::system_clock::now();
    for (int i = 0; i < nz; i++) {
        search.search(i, 1 + dist[from][i], 0, 0, 0);
    }
    auto end = chrono::system_clock::now();

    cout << search.best << endl;
    cout << "in time " << chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << "ms" << endl;
    return 0;
}
