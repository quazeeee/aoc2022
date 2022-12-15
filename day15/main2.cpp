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

using namespace std;

struct SensorData
{
    int _sx;
    int _sy;
    int _bx;
    int _by;
    int _maxD;

    void calculateD()
    {
        _maxD = abs(_sx - _bx) + abs(_by - _sy);
    }

    optional<pair<int, int>> getRangeX(int y)
    {
        int distanceFromS = abs(_sy - y);
        if (distanceFromS > _maxD)
            return std::nullopt;
        int width = _maxD - distanceFromS;
        return optional(pair<int, int>{_sx - width, _sx + width});
    }
};

int64_t calculatePositionsWihoutBeacon(const vector<string> &lines)
{
    vector<SensorData> data;

    for (int i = 0; i < lines.size(); ++i)
    {
        auto &str = lines[i];
        SensorData sensorData;
        auto beginPos = str.find_first_of('=');
        auto endPos = str.find_first_of(',');
        sensorData._sx = stoi(lines[i].substr(beginPos + 1, endPos - beginPos - 1));

        beginPos = str.find_first_of('=', endPos);
        endPos = str.find_first_of(':', beginPos);
        sensorData._sy = stoi(lines[i].substr(beginPos + 1, endPos - beginPos - 1));

        beginPos = str.find_first_of('=', endPos);
        endPos = str.find_first_of(',', beginPos);
        sensorData._bx = stoi(lines[i].substr(beginPos + 1, endPos - beginPos - 1));

        beginPos = str.find_first_of('=', endPos);
        sensorData._by = stoi(lines[i].substr(beginPos + 1));
        data.emplace_back(sensorData);
    }

    for (int i = 0; i < data.size(); ++i)
    {
        data[i].calculateD();
    }

    for (int Y = 0; Y <= 4000000; ++Y)
    {
        vector<pair<int, int>> ranges;

        for (int i = 0; i < data.size(); ++i)
        {
            if (auto rangeX = data[i].getRangeX(Y))
            {
                ranges.push_back(*rangeX);
            }
        }
        std::sort(ranges.begin(), ranges.end(), [](const auto &rangeL, const auto &rangeR)
                  { return rangeL.first < rangeR.first; });

        int lastLeftX = max(ranges.front().first, 0);
        for (int i = 0; i < ranges.size(); ++i)
        {
            if (i && ranges[i].second <= lastLeftX)
                continue;
            if (i && ranges[i].first <= lastLeftX)
            {
                lastLeftX = ranges[i].second;
            }
            else
            {
                if (i)
                {
                    return int64_t(ranges[i].first - 1) * 4000000 + Y;
                }
                lastLeftX = ranges[i].second;
            }
        }
    }
    return 0;
}

int main()
{
    auto fs = ifstream("day15/input.txt", std::ios::in);
    std::string line;
    vector<string> lines;
    while (fs.good() && std::getline(fs, line))
    {
        lines.emplace_back(line);
    }

    cout << calculatePositionsWihoutBeacon(lines);

    return 0;
}