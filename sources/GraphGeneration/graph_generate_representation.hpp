#ifndef GraphGenerateRepresentation_hpp
#define GraphGenerateRepresentation_hpp

#include "../GraphOperation/graph_representation.hpp"

class GraphGenerateRepresentation: public GraphRepresentation {
public:
    //Constructors and a destructor.
    GraphGenerateRepresentation();
    GraphGenerateRepresentation(EmbeddedGraph* _graph);
    ~GraphGenerateRepresentation();

    bool setBestRepresentationIfLastBest();
    bool canEdegMakeBest(EmbeddedEdge* _ed);

protected:

private:
    int lastCount, otherCount;
    int minstart, maxend;
    bool graphIrreducible;
    EmbeddedEdge* lastList[GraphProperty::MAX_EDGE];
    EmbeddedEdge* otherList[GraphProperty::MAX_EDGE];

    void setCandidateLast();
    bool setCandidateOther();

};

#endif
