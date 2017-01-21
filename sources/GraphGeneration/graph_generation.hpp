#ifndef GraphGeneration_hpp
#define GraphGeneration_hpp

#include "split_list.hpp"
#include "graph_generate_representation.hpp"
#include "../GraphOperation/autohomeomorphism.hpp"
#include "../Utilities/file_save_dispatcher.hpp"

class GraphGeneration {
public:
    //A constructor and a destructor.
    GraphGeneration();
    ~GraphGeneration();

    //Methods to generate graphs.
    int generateStart(MutableGraph* _graph);
    void setSaveSetting(string _saveDirectoryPath, string _commonName) { fileSaveDispatcher.setSaveSetting(_saveDirectoryPath, _commonName); }

protected:
    MutableGraph* graph;
    SplitList* spList;
    Autohomeomorphism aut;
    GraphGenerateRepresentation graphRepresentation;
    int generatedNum;
    FileSaveDispatcher fileSaveDispatcher;

    virtual bool isContinue() const { return (graph->getVerticesNum() < SPLIT_MAX); }
    virtual void continueAction() {}
    virtual void terminateAction() { fileSaveDispatcher.save(graph->toSaveGraph()); generatedNum++; }

private:
    int SPLIT_MAX = 10;

    void doSplittingLoop();

};

#endif
