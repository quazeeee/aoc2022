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
#include <execution>

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

int findDecoderKey(vector<string>& packets)
{
    
    std::sort(packets.begin(), packets.end(),[](const string& lhs,const string& rhs){
        return compare(lhs, rhs);
    });

    auto it1 = std::find(packets.begin(), packets.end(), "[[2]]");
    auto it2 = std::find(packets.begin(), packets.end(), "[[6]]");

    return (std::distance(packets.begin(), it1) + 1) * (std::distance(packets.begin(), it2) + 1);
}

int main()
{
    auto fs = ifstream("day13/input.txt", std::ios::in);
    std::string line;
    vector<string> packets;
    while (fs.good() && std::getline(fs, line))
    {
        packets.emplace_back(line);
        std::getline(fs, line);
        packets.emplace_back(line);
        std::getline(fs, line);
    }

    packets.insert(packets.begin(), {"[[2]]", "[[6]]"});

    cout << findDecoderKey(packets);

    return 0;
}