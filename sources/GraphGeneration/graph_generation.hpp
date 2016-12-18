#ifndef GraphGeneration_hpp
#define GraphGeneration_hpp

#include "split_list.hpp"
#include "graph_generate_representation.hpp"
#include "../GraphOperation/automorphism.hpp"

class GraphGeneration {
public:
    //A constructor and a destructor.
    GraphGeneration(MutableGraph* _graph);
    ~GraphGeneration();

    //Methods to generate graphs.
    int generateStart();

protected:
    MutableGraph* graph;
    SplitList* spList;
    Automorphism aut;
    GraphGenerateRepresentation graphRepresentation;
    int generatedNum;

    virtual bool isContinue() const { return (graph->getVerticesNum() < SPLIT_MAX); }
    virtual void continueAction() {}
    virtual void terminateAction() { generatedNum++; }

private:
    int SPLIT_MAX = 10;

    void doSplittingLoop();

};

#endif
