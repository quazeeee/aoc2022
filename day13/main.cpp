#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <functional>
#include <ranges>
#include <bitset>
#include <set>
#include <queue>
#include <numeric>
#include <limits.h>
#include <cctype>

using namespace std;

constexpr string symbols = "0123456789[],";
bool compare(const string& left, const string& right)
{
    for (string::size_type leftPos = left.find_first_of(symbols), rightPos = right.find_first_of(symbols);
         leftPos != string::npos && rightPos != string::npos;
         leftPos = left.find_first_of(symbols, leftPos + 1), rightPos = left.find_first_of(symbols, rightPos + 1))
    {
        if (isdigit(left[leftPos]) && isdigit(right[rightPos]))
        {
            leftPos = left.find_first_of("],", leftPos) - 1;
            rightPos = right.find_first_of("],", rightPos) - 1;

            if (leftPos == rightPos && left[leftPos] == right[rightPos])
                continue;

            return leftPos < rightPos ? true : leftPos > rightPos ? false
                                                                  : left[leftPos] < right[rightPos];
        }

        if (left[leftPos] == right[rightPos])
            continue;

        if (right[rightPos] == ']')
            return false;
        if (left[leftPos] == ']')
            return true;

        if (isdigit(left[leftPos]) && right[rightPos] == '[')
        {
            auto afterDigitPos = left.find_first_of("[],", leftPos);
            string newLeft = "[" + left.substr(leftPos, afterDigitPos - leftPos) + "]" + left.substr(afterDigitPos);
            return compare(newLeft, right.substr(rightPos));
        }

        if (isdigit(right[rightPos]) && left[leftPos] == '[')
        {
            auto afterDigitPos = right.find_first_of("],", rightPos);
            string newRight = "[" + right.substr(rightPos, afterDigitPos - rightPos) + "]" + right.substr(afterDigitPos);
            return compare(left.substr(leftPos), newRight);
        }
    }
    return true;
}

int multIndexes(const vector<pair<string, string>> &pairs)
{
    int sumOfRight = 0;
    for (int i = 0; i < pairs.size(); ++i)
    {
        compare(pairs[i].first, pairs[i].second) ? sumOfRight += i + 1 : 0;
    }
    return sumOfRight;
}

int main()
{
    auto fs = ifstream("day13/input.txt", std::ios::in);
    std::string line;
    vector<pair<string, string>> pairs;
    while (fs.good() && std::getline(fs, line))
    {
        pairs.emplace_back(pair<string, string>(line, ""));
        std::getline(fs, line);
        pairs.back().second = line;
        std::getline(fs, line);
    }

    cout << multIndexes(pairs);

    return 0;
}