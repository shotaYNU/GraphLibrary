#include "utility.hpp"

vector<string> Utility::split(const string& _s, char _delim)
{
    vector<string> elems;
    stringstream ss(_s);
    string item;
    while (getline(ss, item, _delim)) {
        if (!item.empty()) {
            elems.push_back(item);
        }
    }
    return elems;
}

void Utility::exchange(int index1, int index2, long long& value)
{
    long long first = 0ULL;
    long long second = 0ULL;

    if((value & (1ULL << index1)) != 0) { first = 1ULL; value -= (1ULL << index1); }
    if((value & (1ULL << index2)) != 0)  { second = 1ULL; value -= (1ULL << index2); }

    value += (first << index2); value += (second << index1);
}

void Utility::allExchange(int index1, int index2, vector<long long>& values)
{
    for (int i = 0; i < values.size(); ++i)
        Utility::exchange(index1, index2, values[i]);
}
