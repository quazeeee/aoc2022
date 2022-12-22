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
#include <sstream>
#include <list>
#include <numeric>
#include <memory>

using namespace std;

unordered_map<int, pair<int, int>> moves =
    {
        {0, {1, 0}},
        {1, {0, 1}},
        {2, {-1, 0}},
        {3, {0, -1}}};

int nextDirection(int currDirection, char turn)
{
    if (turn == ' ')
        return currDirection;
    return (currDirection + 4 + (turn == 'R' ? 1 : -1)) % 4;
}

tuple<int, int, int> getNextCoord(const vector<string> &field, int x, int y, int direction, pair<int, int> move)
{
    int newx = x + move.first;
    int newy = y + move.second;

    if (newx > x && field[newy][newx] == ' ')
    {
        if (y < 50)
        {
            newy = 149 - y;
            newx = x - 50;
            direction += 2;
        }
        else if (y >= 50 && y < 100)
        {
            newy = 49;
            newx = 50 + y;
            direction += 3;
        }
        else if (y >= 100 && y < 150)
        {
            newy = 149 - y;
            newx = 149;
            direction += 2;
        }
        else
        {
            newy = 149;
            newx = y - 100;
            direction += 3;
        }
    }

    if (newx < x && (newx < 0 || field[newy][newx] == ' '))
    {
        if (y < 50)
        {
            newy = 149 - y;
            newx = 0;
            direction -= 2;
        }
        else if (y >= 50 && y < 100)
        {
            newy = 100;
            newx = (y - 50);
            direction -= 1;
        }
        else if (y >= 100 && y < 150)
        {
            newy = 149 - y;
            newx = 50;
            direction -= 2;
        }
        else
        {
            newy = 0;
            newx = y - 100;
            direction -= 1;
        }
    }

    if (newy > y && field[newy][newx] == ' ')
    {
        if (x < 50)
        {
            newy = 0;
            newx = 100 + x;
        }
        else if (x >= 50 && x < 100)
        {
            newx = 49;
            newy = x + 100;
            direction += 1;
        }
        else
        {
            newx = 99;
            newy = x - 50;
            direction += 1;
        }
    }

    if (newy < y && (newy < 0 || field[newy][newx] == ' '))
    {
        if (x < 50)
        {
            newx = 50;
            newy = 50 + x;
            direction -= 3;
        }
        else if (x >= 50 && x < 100)
        {
            newx = 0;
            newy = x + 100;
            direction -= 3;
        }
        else
        {
            newy = 199;
            newx = x - 100;
        }
    }

    return {newx, newy, direction};
}

int64_t foo(vector<string> &field)
{
    string instructions = field.back();
    field.pop_back();

    int width = 0;
    for (auto &s : field)
        width = std::max(width, static_cast<int>(s.size()));
    ++width;
    for (auto &s : field)
        s.resize(width, ' ');

    int y = 0, x = field[0].find_first_of('.');
    int direction = 0;

    string::size_type pos = instructions.find_first_of("0123456789");
    while (pos != string::npos)
    {
        auto endPos = instructions.find_first_of("RL", pos);
        int length = stoi(instructions.substr(pos, endPos - pos));
        char turn = ' ';
        if (endPos != string::npos)
        {
            turn = instructions[endPos];
        }

        pair<int, int> move = moves[direction];
        while (length--)
        {

            pair<int, int> move = moves[direction];

            tuple<int, int, int> nextPossibleCoord = getNextCoord(field, x, y, direction, move);

            if (field[std::get<1>(nextPossibleCoord)][std::get<0>(nextPossibleCoord)] == '.')
            {
                x = std::get<0>(nextPossibleCoord);
                y = std::get<1>(nextPossibleCoord);
                direction = std::get<2>(nextPossibleCoord);
            }
            else
            {
                break;
            }
        }

        direction = nextDirection(direction, turn);

        pos = instructions.find_first_of("0123456789", endPos);
    }

    return 1000 * (y + 1) + 4 * (x + 1) + direction;
}

int main()
{
    auto fs = ifstream("day22/input.txt", std::ios::in);
    std::string line;
    vector<string> lines;
    while (fs.good() && std::getline(fs, line))
    {
        lines.emplace_back(line);
    }

    cout << foo(lines);

    return 0;
}