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

struct Item {
    bool isLeaf;
    int val;
    vector<Item> subItems;
    Item(): isLeaf(false) {}
    Item(int val): isLeaf(true), val(val) {}    
};

Item toSingular(const Item& item) {
    Item listItem;
    listItem.subItems.emplace_back(item);
    return listItem;
}

enum CompareResult { Right, Wrong, Unknown };

CompareResult compare(const Item& a, const Item& b) {
    if (a.isLeaf && b.isLeaf) {
        if (a.val < b.val) return CompareResult::Right;
        else if (a.val > b.val) return CompareResult::Wrong;
        else return CompareResult::Unknown;
    }
    if (a.isLeaf) return compare(toSingular(a), b);
    if (b.isLeaf) return compare(a, toSingular(b));

    int i;
    for (i = 0; i < min(a.subItems.size(), b.subItems.size()); i++) {
        auto cmp = compare(a.subItems[i], b.subItems[i]);
        if (cmp != CompareResult::Unknown) return cmp;
    }

    if (i == a.subItems.size() && i < b.subItems.size()) {
        return CompareResult::Right;
    }

    if (i == b.subItems.size() && i < a.subItems.size()) {
        return CompareResult::Wrong;
    }
    
    return CompareResult::Unknown;
}

Item parse(const string& s, int& pos) {
    Item item;
    assert(s[pos++] == '[');

    while (pos < s.length() && s[pos] != ']') {
        if (s[pos] == ',') pos++;

        if (s[pos] == '[') {
            item.subItems.emplace_back(parse(s, pos));            
        } else {
            auto nextPos = s.find_first_of(",]", pos);
            auto val = stoi(s.substr(pos, nextPos - pos));
            item.subItems.emplace_back(val);
            pos = nextPos;
        }        
    }

    pos++;
    return item;
}

void prettyPrint(const Item& item) {
    if (item.isLeaf)  {
        cout << item.val;
    } else {
        cout << "[";
        for (const auto& subItem: item.subItems) {
            prettyPrint(subItem);
            cout << ",";
        }
        cout << "]";
    }
}


int main()
{
    //fstream in("input_test");    
    fstream in("input");
        
    string s;
    vector<Item> items;
    while (getline(in, s)) {
        if (s == "") continue;

        int pos = 0;
        items.emplace_back(parse(s, pos));        
    }

    items.emplace_back(toSingular(toSingular(Item(2))));    
    items.emplace_back(toSingular(toSingular(Item(6))));

    sort(items.begin(), items.end(), [](auto& a, auto& b) {
        return compare(a, b) == CompareResult::Right;
    });

    int result = 1;        
    for (int i = 0; i < items.size(); i++) {
        if (!items[i].isLeaf && items[i].subItems.size() == 1
            && !items[i].subItems[0].isLeaf && items[i].subItems[0].subItems.size() == 1
            && (items[i].subItems[0].subItems[0].val == 2 || items[i].subItems[0].subItems[0].val == 6)) {
                result *= i + 1;
            }

        prettyPrint(items[i]);
        cout << endl;        
    }

    cout << result << endl;
    return 0;        
}
