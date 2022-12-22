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

pair<int, int> getNextCoord(const vector<string> &field, int x, int y, pair<int, int> move)
{
    int newx = x + move.first;
    int newy = y + move.second;

    if (newx > x && field[newy][newx] == ' ')
    {
        newx = field[newy].find_first_of(".#");
    }

    if (newx < x && (newx < 0 || field[newy][newx] == ' '))
    {
        newx = field[newy].find_last_of(".#");
    }

    if (newy > y && field[newy][newx] == ' ')
    {
        newy = 0;
        while (field[newy][newx] == ' ')
        {
            ++newy;
        }
    }

    if (newy < y && (newy < 0 || field[newy][newx] == ' '))
    {
        newy = field.size() - 1;
        while (field[newy][newx] == ' ')
        {
            --newy;
        }
    }

    return {newx, newy};
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

            pair<int, int> nextPossibleCoord = getNextCoord(field, x, y, move);

            if (field[nextPossibleCoord.second][nextPossibleCoord.first] == '.')
            {
                x = nextPossibleCoord.first;
                y = nextPossibleCoord.second;
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