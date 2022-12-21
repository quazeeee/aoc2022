#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ranges>
#include <functional>
#include <unordered_map>
#include <string>
#include <cstdint>
#include <numeric>

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

unordered_map<char, char> inverse_op_left = {{'/', '*'}, {'+', '-'}, {'-', '+'}, {'=', '!'}, {'*', '/'}};

unordered_map<char, char> inverse_op_right = {{'/', '#'}, {'+', '-'}, {'=', '!'}, {'-', '$'}, {'*', '/'}};

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
        monkey.name = lines[i].substr(0, 4);
        if (isdigit(lines[i][6]))
        {
            monkey.number = stoi(lines[i].substr(6));
            if (monkey.name == "humn")
            {
                monkey.reverse = true;
            }
        }
        else
        {
            monkey.deps.push_back(lines[i].substr(6, 4));
            monkey.deps.push_back(lines[i].substr(13));
            monkey.op = lines[i][11];
            if (monkey.name == "root")
            {
                monkey.op = '=';
            }
        }
        monkeys.insert({monkey.name, monkey});
    }

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