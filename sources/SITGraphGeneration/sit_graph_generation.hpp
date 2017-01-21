#ifndef SITGraphGeneration_hpp
#define SITGraphGeneration_hpp

#include "../GraphGeneration/graph_generation.hpp"
#include "sit_mutable_graph.hpp"

class SITGraphGeneration : public GraphGeneration {
public:
    //A constructor and a destructor.
    SITGraphGeneration();
    ~SITGraphGeneration();

protected:
    bool isContinue() const { return ((SITMutableGraph*)graph)->isSIT(); }
    void continueAction() { fileSaveDispatcher.save(graph->toSaveGraph()); generatedNum++; }
    void terminateAction() {}

private:

};

#endif
