#ifndef Autohomeomorphism_hpp
#define Autohomeomorphism_hpp

#include "graph_representation.hpp"

class Autohomeomorphism {
public:
    //Constructors and a destructor.
    Autohomeomorphism();
    ~Autohomeomorphism();

    //Getter methods and setter methods
    int getNormalNum() const { return normalCount; }
    int getMirrorNum() const { return mirrorCount; }
    EmbeddedEdge* getAutosEdge(int _autosCount, int _num) const { return autos[_autosCount][_num]; }

    //Methods to set automorphism.
    void setEdgeAutohomeomorphism(GraphRepresentation* _graphRep);
    void setAllAutohomeomorphism();

protected:

private:
    GraphRepresentation* graphRepresentation;
    EmbeddedEdge* autohomeomorphismNormal[GraphProperty::MAX_EDGE];
    EmbeddedEdge* autohomeomorphismMirror[GraphProperty::MAX_EDGE];
    EmbeddedEdge* autos[2 * GraphProperty::MAX_EDGE][GraphProperty::MAX_EDGE];
    int normalCount, mirrorCount;

    void setAutohomeomorphism(int _count, bool _clockwise, EmbeddedEdge** _auto);

};

#endif
