#ifndef Isomorphism_hpp
#define Isomorphism_hpp

#include "../EmbeddedGraph/embedded_graph.hpp"

class Isomorphism {
public:
    //Constructors and a destructor.
    Isomorphism(EmbeddedGraph* _graph);
    ~Isomorphism();

    //Getter methods and setter methods
    void setMappedAdjacents();
    bool isomorphic(const Isomorphism& isomorphism) const;

protected:

private:
    vector<bool*> adjacent;
    vector<vector<bool*>> mappedAdjacents;
    EmbeddedGraph* graph;

};

#endif
