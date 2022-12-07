#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <functional>
#include <ranges>
#include <bitset>

using namespace std;
using namespace std::views;
using namespace std::ranges;

int main()
{
    auto fs = ifstream("input.txt", std::ios::in);
    std::string line;
    std::getline(fs,line);
    constexpr int N = 14;

    cout << endl << ranges::size(line |
        views::transform([&](char c){return bitset<26>(1 << (int)(c - 'a'));}) |
        adjacent_transform<N>([](auto... bs) { return (... | bs).count(); }) |
        take_while([](auto s) { return s < N;}) | views::reverse) + N;

    return 0;
}