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

void Utility::exchangeLong(int index1, int index2, long long& value)
{
    long long first = 0ULL;
    long long second = 0ULL;

    if((value & (1ULL << index1)) != 0) { first = 1ULL; value -= (1ULL << index1); }
    if((value & (1ULL << index2)) != 0)  { second = 1ULL; value -= (1ULL << index2); }

    value += (first << index2); value += (second << index1);
}

void Utility::exchangeBool(int index1, int index2, bool* value)
{
    int temp = value[index1];
    value[index1] = value[index2];
    value[index2] = temp;
}

void Utility::allExchangeLong(int index1, int index2, vector<long long>& values)
{
    for (int i = 0; i < values.size(); ++i)
        Utility::exchangeLong(index1, index2, values[i]);
}

void Utility::allExchangeBool(int index1, int index2, vector<bool*>& values)
{
    for (int i = 0; i < values.size(); ++i)
        Utility::exchangeBool(index1, index2, values[i]);
}

vector<string> Utility::getFilePathsFromDirectory(string directoryName)
{
    vector<string> filePaths;
    DIR* dp = opendir(directoryName.c_str());

    if (dp != NULL) {
        struct dirent* dent;
        do {
            dent = readdir(dp);
            if (dent != NULL && dent->d_name[0] != '.')
                filePaths.push_back(directoryName + "/" + dent->d_name);
        } while(dent != NULL);
        closedir(dp);
    }

    return filePaths;
}

vector<pair<int, int>> Utility::convertToTranspositions(const vector<int> &_originEmbedding, const vector<int> &_newEmbedding)
{
    map<int, int> indexies;
    for (int i = 0; i < _newEmbedding.size(); ++i)
        indexies[_originEmbedding[i]] = i;

    int prev = 0;
    int next = 0;
    int start = 0;
    vector<pair<int, int>> mapping;

    for (int i = 0; i < _originEmbedding.size(); ++i) {
        start = prev = _originEmbedding[i];
        next = _newEmbedding[i];
        if (indexies[next] == -1) continue;
        while (start != next) {
            mapping.push_back(make_pair(prev, next));
            prev = next;
            next = _newEmbedding[indexies[prev]];
            indexies[prev] = -1;
        }
        indexies[next] = -1;
    }

    return mapping;
}

vector<int> Utility::convertToPermutation(const vector<pair<int, int>>& _transpositions, int _length)
{
    vector<int> permutation;
    for (int i = 0; i < _length; ++i)
        permutation.push_back(i);

    int temp = 0;
    for (auto transposition : _transpositions) {
        vector<int>::iterator iter1 = find(permutation.begin(), permutation.end(), transposition.first);
        int index1 = (int)distance(permutation.begin(), iter1);
        vector<int>::iterator iter2 = find(permutation.begin(), permutation.end(), transposition.second);
        int index2 = (int)distance(permutation.begin(), iter2);
        temp = permutation[index1];
        permutation[index1] = permutation[index2];
        permutation[index2] = temp;
    }

    return permutation;
}

void Utility::nextIndexies(vector<int>& indexies, const vector<int> &indexiesMax)
{
    int currentIndex = (int)(indexies.size() - 1);
    while (currentIndex >= 0) {
        indexies[currentIndex] += 1;
        if (indexies[currentIndex] == indexiesMax[currentIndex]) {
            indexies[currentIndex] = 0;
            currentIndex -= 1;
        } else break;
    }
}
