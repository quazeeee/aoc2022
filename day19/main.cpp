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

using namespace std;

struct OreCounter
{
    int ore = 0;
    int clay = 0;
    int obsidian = 0;
    int geode = 0;

    int oreS = 1;
    int clayS = 0;
    int obsidianS = 0;
    int geodeS = 0;

    void collect()
    {
        ore += oreS;
        clay += clayS;
        obsidian += obsidianS;
        geode += geodeS;
    }

    void unCollect()
    {
        ore -= oreS;
        clay -= clayS;
        obsidian -= obsidianS;
        geode -= geodeS;
    }
};

struct BluePrint
{
    vector<int> oRC;
    vector<int> cRC;
    vector<int> obRC;
    vector<int> gRC;

    OreCounter counter;

    void collect()
    {
        counter.collect();
    }

    void build(int r)
    {
        if (r == 0)
        {
            counter.ore -= oRC[0];
            counter.oreS++;
        }
        if (r == 1)
        {
            counter.ore -= cRC[0];
            counter.clayS++;
        }
        if (r == 2)
        {
            counter.ore -= obRC[0];
            counter.clay -= obRC[1];
            counter.obsidianS++;
        }
        if (r == 3)
        {
            counter.ore -= gRC[0];
            counter.obsidian -= gRC[2];
            counter.geodeS++;
        }
    }

    bool canBuildAll()
    {
        return canBuild(0) && canBuild(1) && canBuild(2) && canBuild(3);
    }

    bool canBuild(int r)
    {
        if (r == 0)
        {
            return oRC[0] <= counter.ore;
        }
        if (r == 1)
        {
            return cRC[0] <= counter.ore;
        }
        if (r == 2)
        {
            return obRC[0] <= counter.ore && obRC[1] <= counter.clay;
        }
        if (r == 3)
        {
            return gRC[0] <= counter.ore && gRC[2] <= counter.obsidian;
        }
        return false;
    }
};

int maxGeode(BluePrint bp, unordered_map<int, int> &state, int time)
{
    if (!time)
        return bp.counter.geode;
    if (state.count(time) && state[time] >= bp.counter.geodeS + 2)
        return 0;

    int res = 0;
    if (bp.canBuild(3))
    {
        BluePrint cbp = bp;
        cbp.collect();
        cbp.build(3);
        res = max(res, maxGeode(cbp, state, time - 1));
    }
    else
    {
        for (int i = 3; i >= 0; --i)
        {
            if (bp.canBuild(i))
            {
                BluePrint cbp = bp;
                cbp.counter.unCollect();
                if(cbp.canBuild(i)) continue;
                cbp.collect();
                cbp.collect();
                cbp.build(i);
                res = max(res, maxGeode(cbp, state, time - 1));
            }
        }
        BluePrint cbp = bp;
        cbp.collect();
        res = max(res, maxGeode(cbp, state, time - 1));
    }
    bp.collect();
    state[time] = bp.counter.geodeS;
    return res;
}

int foo(const vector<string> &lines)
{

    vector<BluePrint> bps;

    for (int i = 0; i < lines.size(); ++i)
    { // Blueprint 1: Each ore robot costs 3 ore. Each clay robot costs 4 ore. Each obsidian robot costs 4 ore and 13 clay. Each geode robot costs 3 ore and 7 obsidian.
        string str = lines[i];
        istringstream ss(str);
        int id, oreRC, clayRC, obsRC, obs2RC, geodeRC, geode2RC;
        ss >> id >> oreRC >> clayRC >> obsRC >> obs2RC >> geodeRC >> geode2RC;
        BluePrint bp;
        bp.oRC = {oreRC, 0, 0};
        bp.cRC = {clayRC, 0, 0};
        bp.obRC = {obsRC, obs2RC, 0};
        bp.gRC = {geodeRC, 0, geode2RC};
        bps.push_back(bp);
    }

    int qualitySum = 0;
    int mult = 1;

    for (int i = 0; i < 3; ++i)
    {
        unordered_map<int, int> state;
        mult *= maxGeode(bps[i], state, 32);
    }

    return mult;
}

int main()
{
    auto fs = ifstream("day19/input.txt", std::ios::in);
    std::string line;
    vector<string> lines;
    while (fs.good() && std::getline(fs, line))
    {
        lines.emplace_back(line);
    }

    cout << foo(lines);

    return 0;
}