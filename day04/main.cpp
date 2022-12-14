#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <functional>
#include <ranges>
#include <bitset>
#include <set>
#include <queue>
#include <numeric>
#include <limits.h>

using namespace std;

pair<int, int> getInterval(const string& interval) {
    auto pos = interval.find_first_of('-');
    return {stoi(interval.substr(0, pos)), stoi(interval.substr(pos+1))};
}

bool isContain(pair<int, int> first, pair<int, int> second) {
    return first.first >= second.first && first.second <= second.second;
}

bool isOverlap(const string& pair) {
    auto pos = pair.find_first_of(",");
    std::pair<int, int> first = getInterval(pair.substr(0,pos));
    std::pair<int,int> second = getInterval(pair.substr(pos + 1));
    return isContain(first, second) || (isContain(second, first));
}

int countOverlapPairs(const vector<string>& pairs) {
    int overlapCount = 0;
    for(int i = 0; i < pairs.size(); ++i) {
        if(isOverlap(pairs[i])) overlapCount++;
    }
    return overlapCount;
}

int main()
{
    auto fs = ifstream("day04/input.txt", std::ios::in);
    std::string line;
    vector<string> lines;
    while (fs.good() && std::getline(fs, line))
    {
        lines.emplace_back(line);
    }


    cout << countOverlapPairs(lines);

    return 0;
}