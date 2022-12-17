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

const vector<vector<vector<bool>>> Rocks = {
    {{true, true, true, true}},
    {{false, true, false}, {true, true, true}, {false, true, false}},
    {{true, true, true}, {true, false, false}, {true, false, false}},
    {{true}, {true}, {true}, {true}},
    {{true, true}, {true, true}}};

// bool isRockCoord(const vector<vector<bool>> &rock, int64_t rockLX, int64_t rockBY, int64_t i, int64_t j)
// {
//     if (i < rockBY || i > rockBY + rock.size() - 1 || j < rockLX || j > rockLX + rock[0].size() - 1)
//         return false;
//     return rock[i - rockBY][j - rockLX];
// }

int64_t restRock(vector<vector<bool>> &field, const vector<vector<bool>> &rock, int64_t startHeight, const string &gas, int64_t &gasIter, int64_t &reduceCounter)
{
    int64_t rockLX = field[0].size() - 2 - rock[0].size();
    int64_t rockBY = startHeight;

    bool isMoving = true;

    while (isMoving)
    {
        isMoving = false;
        const int64_t nextGas = gas[gasIter] == '>' ? -1 : 1;
        gasIter = (gasIter + 1) % gas.size();

        auto isInField = [&]()
        {
            int64_t possibleNextRX = rockLX + rock[0].size() - 1 + nextGas;
            return rockLX + nextGas >= 0 && possibleNextRX < field[0].size();
        };

        auto isPossibleRockOverlap = [&](int64_t addY, int64_t addX)
        {
            int64_t possibleNextLX = rockLX + addX;
            int64_t possibleNextBY = rockBY + addY;
            for (int64_t i = possibleNextBY; i < possibleNextBY + rock.size(); ++i)
            {
                for (int64_t j = possibleNextLX; j < possibleNextLX + rock[0].size(); ++j)
                {
                    if (field[i][j] && rock[i - possibleNextBY][j - possibleNextLX])
                        return false;
                }
            }
            return true;
        };

    

        if (isInField() && (startHeight - 4 < rockBY || isPossibleRockOverlap(0, nextGas)))
        {
            rockLX += nextGas;
        }

        if (startHeight - 3 < rockBY || (isPossibleRockOverlap(-1, 0)))
        {
            rockBY -= 1;
            isMoving = true;
        }
        else
        {
            for (int64_t i = rockBY; i < rockBY + rock.size(); ++i)
            {
                for (int64_t j = rockLX; j < rockLX + rock[0].size(); ++j)
                {
                    field[i][j] = field[i][j] || rock[i - rockBY][j - rockLX];
                }
            }
        }
    }

    auto isLineFormed = [&]()
    {
        for (int64_t i = rockBY; i < rockBY + rock.size(); ++i)
        {
            bool isLine = true;
            for (int64_t j = 0; j < field[0].size(); ++j)
            {
                if (!field[i][j])
                {
                    isLine = false;
                    break;
                };
            }
            if (isLine)
                return  i;
        }
        return -1L;
    };
    
    if (int isLine = isLineFormed(); isLine != -1)
    {
        reduceCounter+=isLine;
        vector<vector<bool>> empty(isLine, vector<bool>(7, false));
        vector<vector<bool>> newField;
        newField.reserve(2023*4);
        newField.insert(newField.end(), field.begin() + isLine, field.end());
        newField.insert(newField.end(), empty.begin(), empty.end());
        swap(field, newField);
        rockBY-=isLine;
        startHeight-=isLine;
    }

    return std::max(startHeight - 4, rockBY + (int64_t)rock.size() - 1);
}

int64_t foo(const vector<string> &lines, int64_t rockIterations)
{
    const string gas = lines[0];
    int64_t gasIter = 0;
    vector<vector<bool>> field(2023 * 4, vector<bool>(7, false));

    for (int64_t i = 0; i < field[0].size(); ++i)
    {
        field[0][i] = true;
    }

    int64_t topPoint = 0;
    int64_t reduceCounter = 0;

    map<vector<int>, pair<int, int>> hashState;

    for (int64_t i = 0; i < rockIterations; ++i)
    {
        const vector<vector<bool>> &rock = Rocks[i % 5];

        topPoint = restRock(field, rock, topPoint + 4, gas, gasIter, reduceCounter);
        vector<int> state(9, 0);
        for(int k = 0; k < field[0].size(); ++k) {
            for(int l = topPoint; l >= 0; --l) {
                if(field[l][k]) {
                    state[k+2] = topPoint - l;
                }
            }
        }
        state[0] = gasIter;
        state[1] = i%5;
        if(auto it = hashState.find(state); it != hashState.end()) {
            int64_t distance = i - it->second.first;
            int64_t cyclesCount = ((rockIterations - i - 1)/distance);
            reduceCounter+=  cyclesCount*(topPoint+reduceCounter - it->second.second);
            i += cyclesCount*distance;
        }
        hashState[state] = {i, topPoint + reduceCounter};
    }

    return topPoint + reduceCounter;
}

int main()
{
    auto fs = ifstream("day17/input.txt", std::ios::in);
    std::string line;
    vector<string> lines;
    while (fs.good() && std::getline(fs, line))
    {
        lines.emplace_back(line);
    }

    cout << foo(lines, 2022) << endl;
    cout << foo(lines, 1000000000000) << endl;

    return 0;
}