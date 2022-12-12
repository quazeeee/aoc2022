#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <functional>
#include <ranges>
#include <bitset>

using namespace std;

int64_t isItVisible(int i, int j, vector<vector<int>> &m)
{
    int64_t score = 1;
    int tempScore = 0;
    for (int k = i + 1; k < m.size(); ++k)
    {
        ++tempScore;
        if (m[k][j] >= m[i][j])
        {
            break;
        }
    }
    score *= tempScore;
    tempScore = 0;

    for (int k = i - 1; k >= 0; --k)
    {
        ++tempScore;
        if (m[k][j] >= m[i][j])
        {
            break;
        }
    }

    score *= tempScore;
    tempScore = 0;

    for (int k = j - 1; k >= 0; --k)
    {
        ++tempScore;
        if (m[i][k] >= m[i][j])
        {
            break;
        }
    }

    score *= tempScore;
    tempScore = 0;

    for (int k = j + 1; k < m[i].size(); ++k)
    {
        ++tempScore;
        if (m[i][k] >= m[i][j])
        {
            break;
        }
    }
    score *= tempScore;
    return score;
}

int day8(std::vector<vector<int>> m)
{
    int64_t maxScore = 0;

    for (int i = 0; i < m.size(); ++i)
    {
        for (int j = 0; j < m[i].size(); ++j)
        {
            maxScore = max(maxScore, isItVisible(i, j, m));
        }
    }
    return maxScore;
}

int main()
{
    std::vector<vector<int>> result;
    std::ifstream input("day08/input.txt");
    while (input.good())
    {
        std::string line;
        std::getline(input, line);
        if (!line.empty())
            result.push_back(vector<int>(line.begin(), line.end()));
    }

    std::cout << day8(result) << std::endl;

    return 0;
}