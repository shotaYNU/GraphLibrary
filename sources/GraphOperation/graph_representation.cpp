#include "graph_representation.hpp"

GraphRepresentation::GraphRepresentation()
{
    graph = nullptr;
    repsCount = 0;
    bestRepresentation = new Representation();
    newRepresentation = new Representation();
}

GraphRepresentation::GraphRepresentation(EmbeddedGraph* _graph)
{
    graph = _graph;
    repsCount = 0;
    bestRepresentation = new Representation();
    newRepresentation = new Representation();
}

GraphRepresentation::~GraphRepresentation()
{
    
}

void GraphRepresentation::init(EmbeddedGraph* _graph)
{
    graph = _graph;
    repsCount = 0;
    bestRepresentation = new Representation();
    newRepresentation = new Representation();
}

void GraphRepresentation::setBestRepresentation()
{
    if (graph->isEmpty()) return;

    EmbeddedEdge *ed, *end;
    Representation::Results result;

    bestRepresentation->setRepresentation(graph->getVertex(0)->getFirstEdge(), true);
    for (int i = 0; i < graph->getVerticesNum(); ++i) {
        ed = end = graph->getVertex(i)->getFirstEdge();
        do {
            for (auto clockwise: {true, false}) {
                newRepresentation->setRepresentation(ed, clockwise);
                result = bestRepresentation->compareRepresentation(*newRepresentation);
                if (result == Representation::Results::AUTOMORPHISM) {
                    representations[repsCount++] = new Representation(*newRepresentation);
                } else if (result == Representation::Results::BETTER) {
                    repsCount = 0;
                    representations[repsCount++] = new Representation(*newRepresentation);
                    setNewBestRepresentation();
                }
            }
            ed = ed->getNext();
        } while (ed != end);
    }
}

Representation::Results GraphRepresentation::compareRepresentation(const GraphRepresentation& _graph_rep) const
{
    return bestRepresentation->compareRepresentation(*_graph_rep.bestRepresentation);
}
