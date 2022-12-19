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

struct Search {
    const int MAX_TIME = 26;
    int nz;
    int best;
    unordered_map<long, int> bestByState;    
    const vector<int>& flow;
    const vector<vector<int>>& graph;
    const vector<string>& valves;
    int maxMask;

    Search(int nz, const vector<int>& flow, const vector<vector<int>>& graph, const vector<string>& valves)
        : nz(nz), best(0), flow(flow), graph(graph), valves(valves) {
        maxMask = (1 << nz) - 1;        
    }

    int calcTotalRelease(int time, const vector<int>& valveState) {
        int totalRelease = 0;
        for (int i = 0; i < nz; i++)
            if (valveState[i] != 0) totalRelease += (time - valveState[i]) * flow[i];
        return totalRelease;
    }

    long calcMask(const vector<int>& valveState) {
        int mask = 0;
        for (int i = 0; i < nz; i++)
            if (valveState[i] != 0) mask |= (1 << i);            
        return mask;            
    }

    void search(int pos1, int time1, int pos2, int time2, vector<int>& valveState) { 
        auto mask = calcMask(valveState);
        if ((time1 == MAX_TIME && time2 == MAX_TIME) || mask == maxMask) {    
            auto totalRelease = calcTotalRelease(MAX_TIME, valveState);
            if (totalRelease > best) {
                cout << "new record: " << totalRelease << endl;
            }
            best = max(best, totalRelease);
            return;
        }
        
        auto totalRelease = calcTotalRelease(min(time1, time2), valveState);
        auto state = (mask << 22) | (time2 << 17) | (time1 << 12)  | (pos2 << 6) | pos1;
        auto bestIt = bestByState.find(state);
        if (bestIt != bestByState.end() && totalRelease <= bestIt->second) {
            return; // prune
        }
        bestByState[state] = totalRelease;

        // turn valve if possible
        if (valveState[pos1] == 0) {
            valveState[pos1] = time1;
            search(pos1, time1 + 1, pos2, time2, valveState);
            valveState[pos1] = 0;
            return;
        }

        if (valveState[pos2] == 0) {
            valveState[pos2] = time2;
            search(pos1, time1, pos2, time2 + 1, valveState);
            valveState[pos2] = 0;
            return;
        }


        // move
        for (int next = 0; next < nz; next++) {
            if (next != pos1) {
                auto nextTime = min(MAX_TIME, time1 + graph[pos1][next]);
                search(next, nextTime, pos2, time2, valveState);
            }                    
            if (next != pos2) {
                auto nextTime = min(MAX_TIME, time2 + graph[pos2][next]);
                search(pos1, time1, next, nextTime, valveState);
            }
        }
    }
};


regex lineRegex("Valve (\\w+) has flow rate=(\\d+); tunnels? leads? to valves? (.+)");
regex toRegex("(\\w+)");

int main()
{
    //fstream in("input_test");    
    fstream in("input");
        
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
    for (int i = 0; i < nz; i++) 
        for (int j = i+1; j < nz; j++) {
            cout << "start at " << i << ", " << j << " (out of " << nz << ", " << nz << ")" << endl;
            vector<int> valveState(nz);
            search.search(i, 1 + dist[from][i], j, 1 + dist[from][j], valveState);
    }
    auto end = chrono::system_clock::now();

    cout << search.best << endl;
    cout << "in time " << chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << "ms" << endl;
    return 0;
}
