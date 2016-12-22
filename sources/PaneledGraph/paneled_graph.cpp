#include "paneled_graph.hpp"

PaneledGraph::PaneledGraph()
{
    
}

PaneledGraph::~PaneledGraph()
{
    
}

bool PaneledGraph::isPIT()
{
    initContractible();
    for (int i = 0; i < verticesNum; ++i)
        if (vertices[i]->getIsContractible() && isFlat(i))
            return false;

    return true;
}

bool PaneledGraph::isFlat(int _id)
{
    EmbeddedEdge *ed, *end;

    ed = end = vertices[_id]->getFirstEdge();
    do {
        if (!((PaneledFace*)ed->getNextFace())->getPanel())
            return false;
        ed = ed->getNext();
    } while(ed != end);

    return true;
}
