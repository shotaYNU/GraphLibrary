#ifndef Isomorphism_hpp
#define Isomorphism_hpp

#include "../EmbeddedGraph/embedded_graph.hpp"

class Isomorphism {
public:
    enum Results {
        FAIL = -1,
        EQUIVALENT = 0,
        BETTER = 1,
    };

    //Constructors and a destructor.
    Isomorphism(EmbeddedGraph* _graph);
    ~Isomorphism();

    //Getter methods and setter methods
    void setMappedAdjacents();
    bool isomorphic(const Isomorphism& isomorphism) const;

protected:
    int bestAdjacentRepresentation[GraphProperty::MAX_EDGE];
    int repCount;
    EmbeddedGraph* graph;

    Results compareAdjacent(const int(&_adj1)[GraphProperty::MAX_EDGE], const int(&_adj2)[GraphProperty::MAX_EDGE]) const;
    void setAdjacentRepresentation(const vector<bool*>& _adjacent, int(&_newAdjacentRepresentation)[GraphProperty::MAX_EDGE]);

private:

};

#endif
