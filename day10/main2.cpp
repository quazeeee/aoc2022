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

void print_chunks(auto view, std::string_view separator = " ")
{
    for (auto const subrange : view)
    {
        std::cout << endl;
        for (std::string_view prefix = ""; auto const &elem : subrange)
        {
            std::cout << prefix << elem;
            prefix = separator;
        }
    }
    std::cout << '\n';
}

int main()
{
    auto fs = ifstream("day10/input.txt", std::ios::in);
    std::string line;
    vector<int> instructions{1};
    while(fs.good() && std::getline(fs,line)) {
        instructions.push_back(instructions.empty() ? 1 : instructions.back());
        if(line.substr(0,4) != "noop") {
            instructions.push_back(std::stoi(line.substr(5)) + (instructions.empty() ? 1 : instructions.back()));
        }
    }

    auto sum = views::zip_transform([](auto a, auto b) { return abs(a-b) <= 1 ? 'X' : ' '; },
    views::repeat(iota(0, 40), 6) 
    | views::join , instructions) 
    | chunk(40);

    print_chunks( sum);
    return 0;
}