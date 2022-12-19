#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <functional>
#include <ranges>
#include <limits.h>
#include <optional>
#include <map>
#include <set>
#include <string>
#include <cstdint>
#include <tuple>

using namespace std;

vector<vector<vector<int>>> pocketState(23, vector<vector<int>>(23, vector<int>(23, 0)));

void flood(vector<vector<vector<bool>>> &grid, int i, int j, int k)
{
    if (i < 0 || j < 0 || k < 0 || i > 22 || j > 22 || k > 22)
        return;
    if (grid[i][j][k])
        return;

    if (pocketState[i][j][k] == 1)
        return;

    pocketState[i][j][k] = 1;

    flood(grid, i - 1, j, k);
    flood(grid, i + 1, j, k);
    flood(grid, i, j + 1, k);
    flood(grid, i, j - 1, k);
    flood(grid, i, j, k - 1);
    flood(grid, i, j, k + 1);
};

int64_t foo(const vector<string> &lines)
{
    vector<vector<int>> cubes;
    for (int i = 0; i < lines.size(); ++i)
    {
        vector<int> coords;
        for (const auto word : (std::views::split(lines[i], string_view(","))))
        {
            string coord = std::string{word.begin(), word.end()};
            coords.push_back(stoi(coord));
        }
        cubes.push_back(coords);
    }
    vector<vector<vector<bool>>> grid(23, vector<vector<bool>>(23, vector<bool>(23, false)));

    for (int i = 0; i < cubes.size(); ++i)
    {
        grid[cubes[i][0]][cubes[i][1]][cubes[i][2]] = true;
    }

    int sideCounter = 0;

    auto openSides = [&](int i, int j, int k)
    {
        int count = 6;
        if (i - 1 >= 0 && grid[i - 1][j][k])
            --count;
        if (j - 1 >= 0 && grid[i][j - 1][k])
            --count;
        if (k - 1 >= 0 && grid[i][j][k - 1])
            --count;
        if (i + 1 <= 22 && grid[i + 1][j][k])
            --count;
        if (j + 1 <= 22 && grid[i][j + 1][k])
            --count;
        if (k + 1 <= 22 && grid[i][j][k + 1])
            --count;
        return count;
    };

    for (int i = 0; i < grid.size(); i+=22)
    {
        for (int j = 0; j < grid[0].size(); j+=22)
        {
            for (int k = 0; k < grid[0][0].size(); k+=22)
            {
                flood(grid, i, j, k);
            }
        }
    }

    for (int i = 0; i < grid.size(); ++i)
    {
        for (int j = 0; j < grid[0].size(); ++j)
        {
            for (int k = 0; k < grid[0][0].size(); ++k)
            {
                grid[i][j][k] = grid[i][j][k] || (pocketState[i][j][k] == 0);
            }
        }
    }

    for (int i = 0; i < grid.size(); ++i)
    {
        for (int j = 0; j < grid[0].size(); ++j)
        {
            for (int k = 0; k < grid[0][0].size(); ++k)
            {
                if (grid[i][j][k])
                {
                    sideCounter += openSides(i, j, k);
                }
            }
        }
    }

    return sideCounter;
}

int main()
{
    auto fs = ifstream("day18/input.txt", std::ios::in);
    std::string line;
    vector<string> lines;
    while (fs.good() && std::getline(fs, line))
    {
        lines.emplace_back(line);
    }

    cout << foo(lines) << endl;
    return 0;
}