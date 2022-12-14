#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <functional>
#include <ranges>
#include <numeric>
#include <limits.h>

using namespace std;

pair<int, int> toPair(string_view str)
{
    auto delimPos = str.find_first_of(',');
    return {stoi(string(str.substr(0, delimPos))), stoi(string(str.substr(delimPos + 1)))};
}

pair<int, int> nextSandPos(const pair<int, int>& pos, vector<vector<bool>>& field) {
    if(field[pos.first][pos.second + 1]) return {pos.first, pos.second +1};
    if(field[pos.first - 1][pos.second + 1]) return {pos.first - 1, pos.second +1};
    if(field[pos.first + 1][pos.second + 1]) return {pos.first + 1, pos.second +1};
    return pos;
}

bool restOneSand(vector<vector<bool>>& field, pair<int, int> initPos) {
    pair<int, int> sandPos = initPos;
    do {
        pair<int,int> nextPos = nextSandPos(sandPos, field);
        
        if(nextPos == sandPos && nextPos == initPos && field[nextPos.first][nextPos.second] == false) return false;

        if(nextPos == sandPos) {
            field[nextPos.first][nextPos.second] = false;
            return true;
        }

        sandPos = nextPos;
    } while(true);
    
    return false;
}

int calculateRestSendUnits(const vector<string> &lines)
{
    vector<vector<pair<int, int>>> rocks;

    pair<int, int> leftBottom{INT_MAX, 0};
    pair<int, int> rightTop{INT_MIN, 0};

    for (int i = 0; i < lines.size(); ++i)
    {
        vector<pair<int, int>> rockLine;
        for (const auto word : (std::views::split(lines[i], string_view(" -> "))))
        {
            rockLine.push_back(toPair(std::string_view{word.begin(), word.end()}));
            leftBottom.first = min(leftBottom.first, rockLine.back().first);
            leftBottom.second = max(leftBottom.second, rockLine.back().second);
            rightTop.first = max(rightTop.first, rockLine.back().first);
        }
        rocks.push_back(rockLine);
    }
    
    leftBottom.second+=2;
    int maxSide = max(rightTop.first - leftBottom.first, leftBottom.second - rightTop.second);
    leftBottom.first = 500 - leftBottom.second;
    rightTop.first = 500 + leftBottom.second;
    rocks.push_back({{leftBottom.first - 1,leftBottom.second},{rightTop.first +1,leftBottom.second}});

    vector<vector<bool>> field(rightTop.first - leftBottom.first + 1 + 2, vector<bool>(leftBottom.second - rightTop.second + 1, true));
    for (const auto &rockPath : rocks)
    {
        for (int i = 1; i < rockPath.size(); ++i)
        {
            if (rockPath[i - 1].first == rockPath[i].first)
            {
                for (int k = min(rockPath[i - 1].second, rockPath[i].second); k <= max(rockPath[i - 1].second, rockPath[i].second); ++k)
                {
                    field[rockPath[i].first - leftBottom.first + 1][k] = false;
                }
            }
            else
            {
                for (int k = min(rockPath[i - 1].first, rockPath[i].first); k <= max(rockPath[i - 1].first, rockPath[i].first); ++k)
                {
                    field[k - leftBottom.first + 1][rockPath[i].second] = false;
                }
            }
        }
    }

    int sandCounter = 0;
    while(restOneSand(field, {500 - leftBottom.first + 1, 0})) {
        ++sandCounter;
    }

    return sandCounter;
}

int main()
{
    auto fs = ifstream("day14/input.txt", std::ios::in);
    std::string line;
    vector<string> lines;
    while (fs.good() && std::getline(fs, line))
    {
        lines.emplace_back(line);
    }

    cout << calculateRestSendUnits(lines);

    return 0;
}