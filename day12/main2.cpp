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

pair<int, int> getInitPos(const vector<vector<char>> &map)
{
    for (int i = 0; i < map.size(); i++)
    {
        for (int j = 0; j < map[0].size(); ++j)
        {
            if (map[i][j] == 'S')
            {
                return {i, j};
            }
        }
    }
    return {0, 0};
}

int stepsToE(const vector<vector<char>> &map, vector<vector<int>> &steps, pair<int, int> currPos)
{
    if (map[currPos.first][currPos.second] == 'E')
    {
        return steps[currPos.first][currPos.second];
    }
    std::vector<int> options(4, INT_MAX);

    vector<pair<int, int>> shifts{{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    for (int i = 0; i < shifts.size(); ++i)
    {
        char currS = map[currPos.first][currPos.second];
        if (currPos.first + shifts[i].first < map.size() && currPos.second + shifts[i].second < map[0].size() && currPos.first + shifts[i].first >= 0 && currPos.second + shifts[i].second >= 0 &&
            (((currS - 'a')+ 1 >= (map[currPos.first + shifts[i].first][currPos.second + shifts[i].second] - 'a') && map[currPos.first + shifts[i].first][currPos.second + shifts[i].second] != 'E') || (currS == 'z' && map[currPos.first + shifts[i].first][currPos.second + shifts[i].second] == 'E' )))
        {
            int currSteps = steps[currPos.first][currPos.second];
            if(currS == 'a' && map[currPos.first + shifts[i].first][currPos.second + shifts[i].second] != 'a') {
                currSteps = 0;
            }
            int nextSteps = steps[currPos.first + shifts[i].first][currPos.second + shifts[i].second];
            if (currSteps + 1 < nextSteps)
            {
                steps[currPos.first + shifts[i].first][currPos.second + shifts[i].second] = currSteps + 1;
                options[i] = stepsToE(map, steps, {currPos.first + shifts[i].first, currPos.second + shifts[i].second});
            }
        }
    }

    return *min_element(options.begin(), options.end());
}

int main()
{
    auto fs = ifstream("day12/input.txt", std::ios::in);
    std::string line;
    vector<vector<char>> map;
    while (fs.good() && std::getline(fs, line))
    {
        map.emplace_back(vector<char>(line.begin(), line.end()));
    }

    vector<vector<int>> steps(map.size(), vector<int>(map[0].size(), INT_MAX));

    pair<int, int> currPos = getInitPos(map);
    steps[currPos.first][currPos.second] = 0;
    map[currPos.first][currPos.second] = 'a';

    cout << stepsToE(map, steps, currPos);

    return 0;
}