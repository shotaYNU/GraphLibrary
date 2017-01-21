#ifndef Utility_hpp
#define Utility_hpp

#include <vector>
#include <sstream>
#include <dirent.h>
#include <map>

using namespace std;

class Utility {
public:
    static vector<string> split(const string& _s, char _delim);
    static void exchangeLong(int index1, int index2, long long& value);
    static void exchangeBool(int index1, int index2, bool* value);
    static void allExchangeLong(int index1, int index2, vector<long long>& values);
    static void allExchangeBool(int index1, int index2, vector<bool*>& values);
    static vector<string> getFilePathsFromDirectory(string directoryName);
    static vector<pair<int, int>> convertToTranspositions(const vector<int>& _originEmbedding, const vector<int>& _newEmbedding);
    static vector<int> convertToPermutation(const vector<pair<int, int>>& _transpositions, int _length);
    static void nextIndexies(vector<int>& _indexies, const vector<int>& _indexiesMax);

};

#endif
