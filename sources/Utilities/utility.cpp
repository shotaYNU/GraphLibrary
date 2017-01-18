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
