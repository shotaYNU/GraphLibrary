#ifndef GraphRepresentation_hpp
#define GraphRepresentation_hpp

#include "../EmbeddedGraph/embedded_graph.hpp"
#include "breadth_first_search.hpp"
#include "representation.hpp"

class GraphRepresentation {
public:
    int repsCount;
    EmbeddedGraph* graph;

    //Constructors and a destructor.
    GraphRepresentation();
    GraphRepresentation(EmbeddedGraph* _graph);
    ~GraphRepresentation();

    void setBestRepresentation();
    Representation* getRepresentation(int _num) const { return representations[_num]; }
    Representation* getBestRepresentation() const { return bestRepresentation; }
    void init(EmbeddedGraph* _graph);

    //A method to compare representation.
    Representation::Results compareRepresentation(const GraphRepresentation& _graph_rep) const;

protected:
    Representation* bestRepresentation;
    Representation* newRepresentation;
    Representation* representations[GraphProperty::MAX_EDGE];

    virtual void setNewBestRepresentation() { bestRepresentation = new Representation(*newRepresentation); }

private:

};

#endif
