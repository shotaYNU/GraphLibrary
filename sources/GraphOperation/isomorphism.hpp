#ifndef Isomorphism_hpp
#define Isomorphism_hpp

#include "../EmbeddedGraph/embedded_graph.hpp"
#include <array>

class Isomorphism {
public:
    enum Results {
        FAIL = -1,
        EQUIVALENT = 0,
        BETTER = 1,
    };
    static const int ADJACENT_MAX = 20;

    //Constructors and a destructor.
    Isomorphism(EmbeddedGraph* _graph);
    ~Isomorphism();

    //Getter methods and setter methods
    void setMappedAdjacents();
    bool isomorphic(const Isomorphism& isomorphism) const;

protected:
    array<int, GraphProperty::MAX_EDGE> bestAdjacentRepresentation;
    int repCount;
    EmbeddedGraph* graph;

    Results compareAdjacent(const array<int, GraphProperty::MAX_EDGE>& _adj1, const array<int, GraphProperty::MAX_EDGE>& _adj2) const;
    void setAdjacentRepresentation(const vector<bool*>& _adjacent, array<int, GraphProperty::MAX_EDGE>& _newAdjacentRepresentation);

private:

};

#endif
