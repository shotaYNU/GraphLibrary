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