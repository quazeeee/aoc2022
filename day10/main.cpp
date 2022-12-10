#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <functional>
#include <ranges>
#include <bitset>
#include <set>

using namespace std;
using namespace std::views;
using namespace std::ranges;

int main()
{
    auto fs = ifstream("day10/input.txt", std::ios::in);
    std::string line;
    vector<int> instructions;
    while(fs.good() && std::getline(fs,line)) {
        instructions.push_back(instructions.empty() ? 1 : instructions.back());
        if(line.substr(0,4) != "noop") {
            instructions.push_back(std::stoi(line.substr(5)) + (instructions.empty() ? 1 : instructions.back()));
        }
    }

    cout << (iota(20, 221)
    | filter([](int i) {return i%20 == 0 && i%40 != 0;})
    | views::transform([&](int i ) {return (instructions| take(i - 1)
    | drop (i-2)).front() * i;})
    | adjacent_transform<6>([](auto... ints) { return (... + ints); })).front();

    return 0;
}