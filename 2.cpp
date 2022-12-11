#include <iostream>
#include <limits>
#include <queue>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

enum Move {Rock, Paper, Scissors};

enum Result { First, Second, Draw};

const static unordered_map<string, Move> symbolMap = {
        {"A", Move::Rock},
        {"B", Move::Paper},
        {"C", Move::Scissors},
    };


const static unordered_map<string, Result> resultMap = {
        {"X", Result::Second},
        {"Y", Result::Draw},
        {"Z", Result::First},
    };

// second => result => first
const static unordered_map<Move, unordered_map<Result, Move>> rules = {
    {
        Move::Rock, {
            { Result::Draw, Move::Rock },
            { Result::First, Move::Paper},
            { Result::Second, Move::Scissors }
        }
    },
    {
        Move::Paper, {
            { Result::Draw, Move::Paper },
            { Result::First, Move::Scissors},
            { Result::Second, Move::Rock }
        }
    },
    {
        Move::Scissors, {
            { Result::Draw, Move::Scissors },
            { Result::First, Move::Rock},
            { Result::Second, Move::Paper }
        }
    }
};

int moveScore(Move move) {
    switch (move) {
        case Move::Rock: return 1;
        case Move::Paper: return 2;
        case Move::Scissors: return 3;
    }
    throw "oops";
}

int resultScore(Result result) {
    switch (result) {
        case Result::First: return 6;
        case Result::Second: return 0;
        case Result::Draw: return 3;
    }
    throw "oops";
}

int main()
{
    fstream in("input_test");
    
    string s;
    int score = 0; 
    while (in.peek() != EOF) {
        in >> s;
        auto opponent = symbolMap.find(s)->second;

        in >> s;
        auto result = resultMap.find(s)->second;

        auto you = rules.find(opponent)->second.find(result)->second;

        score += moveScore(you);
        score += resultScore(result);
    }        
    cout << score << endl;
}
