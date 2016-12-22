#include "paneled_graph_representation.hpp"

PaneledGraphRepresentation::PaneledGraphRepresentation()
{
    graph = nullptr;
    repsCount = 0;
    bestRepresentation = new PaneledRepresentation();
    newRepresentation = new PaneledRepresentation();
}

PaneledGraphRepresentation::PaneledGraphRepresentation(PaneledGraph* _graph)
{
    graph = _graph;
    repsCount = 0;
    bestRepresentation = new PaneledRepresentation();
    newRepresentation = new PaneledRepresentation();
}

PaneledGraphRepresentation::~PaneledGraphRepresentation()
{
    
}
