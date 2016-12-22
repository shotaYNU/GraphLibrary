#ifndef Utility_hpp
#define Utility_hpp

#include <vector>
#include <sstream>

using namespace std;

class Utility {
public:
    static vector<string> split(const string& _s, char _delim);
    static void exchange(int index1, int index2, long long& value);
    static void allExchange(int index1, int index2, vector<long long>& values);

};

#endif
