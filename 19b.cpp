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

enum Resource {
    Ore = 0,
    Clay = 1,
    Obsidian = 2,
    Geode = 3,
};

using Composition = vector<int>;
using Blueprint = vector<Composition>;

const int RESOURCE_COUNT = 4;

const int MAX_TIME = 32;

const vector<string> resourceNames = { "ore", "clay", "obsidian", "geode"};

static int maxRes = 0;

void search(int time, int result, int& best, vector<int>& resources, vector<int>& robots, const Blueprint& blueprint, const vector<int>& maxReq, unordered_map<long long, int>& memo) {    
    if (time > MAX_TIME) return;

    auto remaining = MAX_TIME - time + 1;
    if (result + remaining * (remaining - 1)/2 <= best) return;

    long long key = 0;
    for (int r = 0; r < RESOURCE_COUNT-1; r++) key = (key << 14) | resources[r];
    for (int r = 0; r < RESOURCE_COUNT-1; r++) key = (key << 5) | robots[r];
    key = (key << 6) | time;
    
    auto prev = memo.find(key);
    if (prev != memo.end() && prev->second >= result) return;
    memo[key] = result;

    for (; time <= MAX_TIME; time++) {       
        for (int robot = 0; robot < RESOURCE_COUNT; robot++) {
            if (resources[robot] + robots[robot]*(MAX_TIME - time) >= maxReq[robot]*(MAX_TIME - time+1)) continue;

            auto possible = true;
            for (int r = 0; r < RESOURCE_COUNT; r++)
                if (resources[r] < blueprint[robot][r]) possible = false;

            if (possible) {                
                auto newResources = resources;
                for (int r = 0; r < RESOURCE_COUNT; r++) newResources[r] += robots[r];
                for (int r = 0; r < RESOURCE_COUNT; r++) newResources[r] -= blueprint[robot][r];

                auto newRobots = robots;
                newRobots[robot]++;
                
                auto newResult = result;
                if (robot == Geode) {
                    newResult += MAX_TIME - time;
                }

                search(time+1, newResult, best, newResources, newRobots, blueprint, maxReq, memo);
            }
        }

        for (int r = 0; r < RESOURCE_COUNT; r++) resources[r] += robots[r];
    }

    if (result > best) {
        best = result;
        cout << "new best " << best << endl;
    } 
}

int eval(const Blueprint& blueprint) {    
    int best = 0;
    vector<int> resources(RESOURCE_COUNT);
    vector<int> robots(RESOURCE_COUNT);
    robots[Ore] = 1;
    vector<int> maxReq(RESOURCE_COUNT);
    maxReq[Geode] = 1000000;
    for (int robot = 0; robot < RESOURCE_COUNT; robot++)
        for (int r = 0; r < RESOURCE_COUNT; r++)
            maxReq[r] = max(maxReq[r], blueprint[robot][r]);
    unordered_map<long long, int> memo;
    search(1, 0, best, resources, robots, blueprint, maxReq, memo);
    return best;
}

void calc(const vector<Blueprint>& blueprints) {
    int idx = 1;
    int result = 1;
    for (const auto& blueprint: blueprints) {
        cout << "eval " << idx << endl;

        auto start = chrono::system_clock::now();    
        auto mx = eval(blueprint);
        auto end = chrono::system_clock::now();
        cout << "in time " << chrono::duration_cast<chrono::milliseconds>(end-start).count() << "ms" << endl;

        result *= mx;
        idx++;
    }

    cout << result << endl;
}

Resource parseResource(string s) {
    if (s == "ore") return Ore;
    else if (s == "clay") return Clay;
    else if (s == "obsidian") return Obsidian;
    else return Geode;
}

regex blueprint_regex("Blueprint (\\d+): (.*)");
regex robot_regex("Each (\\w+) robot costs ([^\\.]+)\\.");
regex composition_regex("(\\d+) (\\w+)");

int main()
{
    //fstream in("input_test");    
    fstream in("input");

    string s;
    vector<Blueprint> blueprints;
    while (getline(in, s)) {
        Blueprint blueprint(RESOURCE_COUNT);
        
        smatch match;
        regex_match(s, match, blueprint_regex);
        auto blueprintStr = match[2].str();
        
        auto robot_begin = sregex_iterator(blueprintStr.begin(), blueprintStr.end(), robot_regex);
        auto robot_end = sregex_iterator();
        for (auto it = robot_begin; it != robot_end; it++) {
            auto robotType = parseResource((*it)[1]); 
            auto compositionStr = ((*it)[2]).str();            

            Composition composition(RESOURCE_COUNT);
            auto comp_begin = sregex_iterator(compositionStr.begin(), compositionStr.end(), composition_regex);
            auto comp_end = sregex_iterator();
            for (auto it = comp_begin; it != comp_end; it++) {
                auto amount =  stoi((*it)[1]);
                auto component = parseResource((*it)[2]);
                composition[component] = amount;
            }
            blueprint[robotType] = composition;
        }        
        blueprints.emplace_back(blueprint);
    } 


    if (blueprints.size() > 3) blueprints.resize(3);
    calc(blueprints);

    return 0;
}
