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

unordered_map<char, std::function<int64_t(int64_t, int64_t)>> ops = {
    {'*', [](int64_t a, int64_t b)
     { return a * b; }},
    {'/', [](int64_t a, int64_t b)
     { return a / b; }},
    {'#', [](int64_t a, int64_t b)
     { return b / a; }},
    {'+', [](int64_t a, int64_t b)
     { return a + b; }},
    {'-', [](int64_t a, int64_t b)
     { return a - b; }},
    {'$', [](int64_t a, int64_t b)
     { return b - a; }},
    {'!', [](int64_t a, int64_t b)
     { return a; }},
    {'=', [](int64_t a, int64_t b)
     { return static_cast<int64_t>(a == b); }}};

unordered_map<char, char> inverse_op_left = {
    {'/', '*'},
    {'+', '-'},
    {'-', '+'},
    {'=', '!'},
    {'*', '/'}};

unordered_map<char, char> inverse_op_right = {
    {'/', '#'}, // args inversed
    {'+', '-'},
    {'=', '!'},
    {'-', '$'}, // args inversed
    {'*', '/'}};

struct Monkey
{
    string name;
    int64_t number = 0;
    char op = ' ';
    vector<string> deps;
    bool reverse = false;
};

int64_t foo(const vector<string> &lines)
{
    unordered_map<string, Monkey> monkeys;

    for (int64_t i = 0; i < lines.size(); ++i)
    {
        Monkey monkey;
        string mName = lines[i].substr(0, 4);
        monkey.name = mName;
        if (isdigit(lines[i][6]))
        {
            monkey.number = stoi(lines[i].substr(6));
            if (mName == "humn")
            {
                monkey.reverse = true;
            }
        }
        else
        {
            monkey.deps.push_back(lines[i].substr(6, 4));
            monkey.deps.push_back(lines[i].substr(13));
            monkey.op = lines[i][11];
            if (mName == "root")
            {
                monkey.op = '=';
            }
        }
        monkeys.insert({mName, monkey});
    }

    int64_t postOrderNum = 0;

    std::function<void(Monkey &)> dfs;
    dfs = [&](Monkey &mk) -> void
    {
        if (!mk.deps.empty())
        {
            dfs(monkeys[mk.deps[0]]);
            dfs(monkeys[mk.deps[1]]);
            if (monkeys[mk.deps[0]].reverse || monkeys[mk.deps[1]].reverse)
            {
                mk.reverse = true;
                if (mk.name == "root")
                {
                    mk.number = monkeys[mk.deps[0]].reverse ? monkeys[mk.deps[1]].number : monkeys[mk.deps[0]].number;
                }
            }
            else
            {
                mk.number = ops[mk.op](monkeys[mk.deps[0]].number, monkeys[mk.deps[1]].number);
            }
        }
    };

    dfs(monkeys["root"]);

    std::function<int64_t(Monkey &)> dfs2;
    dfs2 = [&](Monkey &mk) -> int64_t
    {
        if (mk.name == "humn")
        {
            return mk.number;
        }
        if (!mk.deps.empty())
        {
            if (monkeys[mk.deps[0]].reverse)
            {
                monkeys[mk.deps[0]].number = ops[inverse_op_left[mk.op]](mk.number, monkeys[mk.deps[1]].number);
                return dfs2(monkeys[mk.deps[0]]);
            }
            else
            {
                monkeys[mk.deps[1]].number = ops[inverse_op_right[mk.op]](mk.number, monkeys[mk.deps[0]].number);
                return dfs2(monkeys[mk.deps[1]]);
            }
        }
        return 0;
    };

    return dfs2(monkeys["root"]);
}

int main()
{
    auto fs = ifstream("day21/input.txt", std::ios::in);
    std::string line;
    vector<string> lines;
    while (fs.good() && std::getline(fs, line))
    {
        lines.emplace_back(line);
    }

    cout << foo(lines);

    return 0;
}