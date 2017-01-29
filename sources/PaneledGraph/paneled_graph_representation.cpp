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

void PaneledGraphRepresentation::setBestRepresentation()
{
    traversedAll = true;
    if (graph->isEmpty()) return;

    EmbeddedEdge *ed, *end;
    Representation::Results result;

    int realCount = 0;
    for (int i = 0; i < graph->getVerticesNum(); ++i)
        if (!((PaneledGraph*)graph)->isFlat(i))
            realCount++;

    bool beforeInit = true;
    for (int i = 0; i < graph->getVerticesNum(); ++i) {
        if (((PaneledGraph*)graph)->isFlat(i)) continue;
        ed = end = graph->getVertex(i)->getFirstEdge();
        do {
            for (auto clockwise: {true, false}) {
                newRepresentation->setRepresentation(ed, clockwise);
                if (((PaneledRepresentation*)newRepresentation)->getVerticesCount() != realCount) {
                    traversedAll = false;
                    return;
                }
                if (beforeInit) {
                    repsCount = 0;
                    representations[repsCount++] = new Representation(*newRepresentation);
                    setNewBestRepresentation();
                    beforeInit = false;
                } else {
                    result = bestRepresentation->compareRepresentation(*newRepresentation);
                    if (result == Representation::Results::AUTOMORPHISM) {
                        representations[repsCount++] = new Representation(*newRepresentation);
                    } else if (result == Representation::Results::BETTER) {
                        repsCount = 0;
                        representations[repsCount++] = new Representation(*newRepresentation);
                        setNewBestRepresentation();
                    }
                }

            }
            ed = ed->getNext();
        } while (ed != end);
    }
}
