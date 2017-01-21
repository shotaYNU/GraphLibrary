#include "autohomeomorphism.hpp"

Autohomeomorphism::Autohomeomorphism()
{
    normalCount = mirrorCount = 0;
    graphRepresentation = nullptr;
}

Autohomeomorphism::~Autohomeomorphism()
{
    
}

void Autohomeomorphism::setEdgeAutohomeomorphism(GraphRepresentation* _graphRep)
{
    normalCount = mirrorCount = 0;
    graphRepresentation = _graphRep;
    for (int i = 0; i < graphRepresentation->repsCount; ++i) {
        Representation* rep = graphRepresentation->getRepresentation(i);
        if (rep->clockwise) {
            autohomeomorphismNormal[normalCount++] = rep->edge;
        } else {
            autohomeomorphismMirror[mirrorCount++] = rep->edge;
        }
    }
}

void Autohomeomorphism::setAutohomeomorphism(int _count, bool _clockwise, EmbeddedEdge** _auto)
{
    EmbeddedEdge* ed = (_clockwise) ? autohomeomorphismNormal[_count] : autohomeomorphismMirror[_count];
    EmbeddedEdge* nowEdge;
    int autoCount = 0;
    BreadthFirstSearch bfsSearch(graphRepresentation->graph);

    bfsSearch.init(ed, _clockwise);
    while (bfsSearch.hasNext()) {
        nowEdge = bfsSearch.next();
        _auto[autoCount++] = nowEdge;
    }
}

void Autohomeomorphism::setAllAutohomeomorphism()
{
    for (int i = 0; i < normalCount; ++i)
        setAutohomeomorphism(i, true, autos[i]);

    for (int i = 0; i < mirrorCount; ++i)
        setAutohomeomorphism(i, false, autos[i + normalCount]);
}
