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
    vector<bool*> getAdjacent() const { return adjacent; }
    vector<vector<bool*>> getMappedAdjacents() const { return mappedAdjacents; }

protected:
    vector<bool*> adjacent;
    vector<vector<bool*>> mappedAdjacents;
    EmbeddedGraph* graph;

    bool equal(const vector<bool*>& _adj1, const vector<bool*>& _adj2) const;

private:

};

#endif
