#ifndef file_save_dispatcher_hpp
#define file_save_dispatcher_hpp

#include <fstream>

using namespace std;

class FileSaveDispatcher {
public:
    FileSaveDispatcher();
    ~FileSaveDispatcher();
    void setSaveSetting(string _saveDirectoryPath, string _commonName) { saveDirectoryPath = _saveDirectoryPath; commonName = _commonName; }
    void save(string _data);

    static void save(string _filePath, string _data);

protected:

private:
    string saveDirectoryPath;
    string commonName;
    int currentSaveNums;

};

#endif
