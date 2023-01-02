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

const int MAX_TIME = 24;

const vector<string> resourceNames = { "ore", "clay", "obsidian", "geode"};

void search(int time, int& best, vector<int>& resources, vector<int>& robots, const Blueprint& blueprint, unordered_set<long long>& seen) {    
    if (time > MAX_TIME) return;

    long long key = 0;
    for (int r = 0; r < RESOURCE_COUNT; r++) {
        key = (key << 5) | resources[r];
        key = (key << 5) | robots[r];
    }
    if (seen.count(key) > 0) return;
    seen.insert(key);

    for (; time <= MAX_TIME; time++) {
        for (int robot = 0; robot < RESOURCE_COUNT; robot++) {
            if (robot == Ore && robots[Ore] >= 4) continue;
            if (robot == Clay && robots[Clay] >= 20) continue;
            if (robot == Obsidian && robots[Obsidian] >= 20) continue;

            auto possible = true;
            for (int r = 0; r < RESOURCE_COUNT; r++)
                if (resources[r] < blueprint[robot][r]) possible = false;

            if (possible) {                
                auto newResources = resources;
                for (int r = 0; r < RESOURCE_COUNT; r++) newResources[r] += robots[r];
                for (int r = 0; r < RESOURCE_COUNT; r++) newResources[r] -= blueprint[robot][r];

                auto newRobots = robots;
                newRobots[robot]++;
                search(time+1, best, newResources, newRobots, blueprint, seen);
            }
        }

        for (int r = 0; r < RESOURCE_COUNT; r++) resources[r] += robots[r];
    }

    if (resources[Geode] > best) {
        best = resources[Geode];
        cout << "new best " << best << endl;
    } 
}

int eval(const Blueprint& blueprint) {    
    int best = 0;
    vector<int> resources(RESOURCE_COUNT);
    vector<int> robots(RESOURCE_COUNT);
    robots[Ore] = 1;
    unordered_set<long long> seen;
    search(1, best, resources, robots, blueprint, seen);
    return best;
}

void calc(const vector<Blueprint>& blueprints) {
    int result = 0;
    auto idx = 1;        
    for (const auto& blueprint: blueprints) {
        cout << "eval " << idx << endl;

        auto start = chrono::system_clock::now();    
        auto mx = eval(blueprint);
        auto end = chrono::system_clock::now();
        cout << "in time " << chrono::duration_cast<chrono::milliseconds>(end-start).count() << "ms" << endl;

        result += idx * mx;
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

    calc(blueprints);

    return 0;
}
