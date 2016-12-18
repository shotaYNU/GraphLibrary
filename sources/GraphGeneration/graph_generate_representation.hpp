#ifndef GraphGenerateRepresentation_hpp
#define GraphGenerateRepresentation_hpp

#include "../GraphOperation/graph_representation.hpp"
#include "../GraphGeneration/mutable_graph.hpp"

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
    int colors[GraphProperty::MAX_VERTEX];

    void setCandidateLast();
    bool setCandidateOther();

};

#endif
