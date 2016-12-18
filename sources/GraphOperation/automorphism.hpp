#ifndef Automorphism_hpp
#define Automorphism_hpp

#include "graph_representation.hpp"

class Automorphism {
public:
    //Constructors and a destructor.
    Automorphism();
    ~Automorphism();

    //Getter methods and setter methods
    int getNormalNum() const { return normalCount; }
    int getMirrorNum() const { return mirrorCount; }
    EmbeddedEdge* getAutosEdge(int _autosCount, int _num) const { return autos[_autosCount][_num]; }

    //Methods to set automorphism.
    void setEdgeAutomorphism(GraphRepresentation* _graphRep);
    void setAllAutomorphism();

protected:

private:
    GraphRepresentation* graphRepresentation;
    EmbeddedEdge* automorphismNormal[GraphProperty::MAX_EDGE];
    EmbeddedEdge* automorphismMirror[GraphProperty::MAX_EDGE];
    EmbeddedEdge* autos[2 * GraphProperty::MAX_EDGE][GraphProperty::MAX_EDGE];
    int normalCount, mirrorCount;

    void setAutomorphism(int _count, bool _clockwise, EmbeddedEdge** _auto);

};

#endif
