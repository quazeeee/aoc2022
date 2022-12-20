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

using namespace std;

struct Number
{
    int64_t num;
    int64_t pos = 0;
};

int64_t foo(const vector<string> &lines)
{

    list<Number> nums;
    vector<list<Number>::iterator> order;

    for (int i = 0; i < lines.size(); ++i)
    {
        auto it = nums.emplace(nums.end(), Number{stoll(lines[i]) * 811589153LL, i});
        order.push_back(it);
    }

    for (int k = 0; k < 10; ++k)
    {
        for (int i = 0; i < order.size(); ++i)
        {
            auto it = order[i];
            int64_t curPos = it->pos;
            int64_t newPos = ((curPos + it->num) % ((int64_t)nums.size() - 1LL) + nums.size() - 1)% (nums.size() - 1);

            if (newPos == 0 && newPos != curPos)
            {
                newPos = nums.size() - 1;
            }

            int64_t diff = newPos - curPos;

            auto changeIt = it;
            if (diff > 0)
            {
                changeIt++;
                while (diff--)
                {
                    changeIt->pos--;
                    changeIt++;
                    changeIt->pos;
                }
            }
            else if (diff < 0)
            {
                while (diff++)
                {
                    changeIt--;
                    changeIt->pos++;
                }
            }
            it->pos = newPos;
            nums.splice(changeIt, nums, it);
        }
    }

    auto it0 = std::find_if(nums.begin(), nums.end(), [](const Number &numl)
                            { return numl.num == 0; });

    int64_t sum = 0;
    int counter = 0;

    while (counter <= 3000)
    {
        counter++;
        ++it0;
        if (counter % 1000 == 0)
        {
            sum += it0->num;
        }
    }

    return sum;
}

int main()
{
    auto fs = ifstream("day20/input.txt", std::ios::in);
    std::string line;
    vector<string> lines;
    while (fs.good() && std::getline(fs, line))
    {
        lines.emplace_back(line);
    }

    cout << foo(lines);

    return 0;
}