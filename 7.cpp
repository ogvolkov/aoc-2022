#include <iostream>
#include <limits>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <deque>

using namespace std;

vector<int> stoints(const string& s) {
    vector<int> result;
    regex num_regex("\\d+");
    auto match_begin = sregex_iterator(s.cbegin(), s.cend(), num_regex);
    auto match_end = sregex_iterator();
    for (auto it = match_begin; it != match_end; it++)
        result.emplace_back(stoi(it->str()));
    return result;
}

struct Dir {
    private:
        Dir* parent;
        vector<int> files;
        unordered_map<string, shared_ptr<Dir>> subdirs;

    public:
        Dir(Dir* parent): parent(parent) {}

        void addFile(int size) {
            files.emplace_back(size);
        }

        Dir* cd(string name) {
            if (name == "..") return parent;
            
            auto subdir = make_shared<Dir>(this);
            subdirs[name] = subdir;
            return subdir.get();
        }

        int dirSizes(vector<int>& accumulator) {            
            int dirSize = 0;
            for (const auto size: files)
                dirSize += size;
            for (const auto& [name, subdir]: subdirs)
                dirSize += subdir->dirSizes(accumulator);

            accumulator.emplace_back(dirSize);
            return dirSize;
        }
};

int main()
{
    //fstream in("input_test");
    fstream in("input");
        
    string s;
    Dir root(nullptr);
    Dir* current = &root;

    regex dirCmdRegex("\\$ cd (.+)");
    regex fileRegex("(\\d+) .+");

    while (getline(in, s)) {
        if (s == "$ cd /") continue;
        if (s == "ls") continue;
        
        smatch matches;
        if (regex_search(s, matches, dirCmdRegex)) {
            auto subdir = matches[1];
            cout << "cd to " << subdir << endl;
            current = current->cd(subdir);
            continue;
        }

        if (regex_search(s, matches, fileRegex)) {
            auto size_str = matches[1];
            cout << "found file of size " << size_str << endl;
            auto size = stoi(size_str);
            current->addFile(size);
            continue;
        }
    }
    vector<int> sizes;
    auto rootSize = root.dirSizes(sizes);

    sort(sizes.begin(), sizes.end());

    for (const auto size: sizes)
        if (70000000 - rootSize + size >= 30000000) {
            cout << size << endl;
            break;
        }    
    
    return 0;        
}
