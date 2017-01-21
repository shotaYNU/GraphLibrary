#include "file_save_dispatcher.hpp"

FileSaveDispatcher::FileSaveDispatcher()
{
    saveDirectoryPath = "";
    commonName = "";
    currentSaveNums = 0;
}

FileSaveDispatcher::~FileSaveDispatcher()
{    
}

void FileSaveDispatcher::save(string _data)
{
    ofstream outfile(saveDirectoryPath + "/" + commonName + to_string(currentSaveNums++) + ".json");
    outfile << _data;
    outfile.close();
}

void FileSaveDispatcher::save(string _filePath, string _data)
{
    ofstream outfile(_filePath);
    outfile << _data;
    outfile.close();
}
