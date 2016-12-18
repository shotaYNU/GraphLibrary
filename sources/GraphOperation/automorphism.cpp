#include "automorphism.hpp"

Automorphism::Automorphism()
{
    normalCount = mirrorCount = 0;
    graphRepresentation = nullptr;
}

Automorphism::~Automorphism()
{
    
}

void Automorphism::setEdgeAutomorphism(GraphRepresentation* _graphRep)
{
    normalCount = mirrorCount = 0;
    graphRepresentation = _graphRep;
    for (int i = 0; i < graphRepresentation->repsCount; ++i) {
        Representation* rep = graphRepresentation->getRepresentation(i);
        if (rep->clockwise) {
            automorphismNormal[normalCount++] = rep->edge;
        } else {
            automorphismMirror[mirrorCount++] = rep->edge;
        }
    }
}

void Automorphism::setAutomorphism(int _count, bool _clockwise, EmbeddedEdge** _auto)
{
    EmbeddedEdge* ed = (_clockwise) ? automorphismNormal[_count] : automorphismMirror[_count];
    EmbeddedEdge* nowEdge;
    int autoCount = 0;
    BreadthFirstSearch bfsSearch(graphRepresentation->graph);

    bfsSearch.init(ed, _clockwise);
    while (bfsSearch.hasNext()) {
        nowEdge = bfsSearch.next();
        _auto[autoCount++] = nowEdge;
    }
}

void Automorphism::setAllAutomorphism()
{
    for (int i = 0; i < normalCount; ++i)
        setAutomorphism(i, true, autos[i]);

    for (int i = 0; i < mirrorCount; ++i)
        setAutomorphism(i, false, autos[i + normalCount]);
}
