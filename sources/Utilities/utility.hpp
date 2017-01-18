#ifndef Utility_hpp
#define Utility_hpp

#include <vector>
#include <sstream>
#include <dirent.h>

using namespace std;

class Utility {
public:
    static vector<string> split(const string& _s, char _delim);
    static vector<string> getFilePathsFromDirectory(string directoryName);

};

#endif
