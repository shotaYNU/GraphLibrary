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

    vector<vector<int>> existTraversedVertices;
    for (int i = 0; i < graph->getVerticesNum(); ++i) {
        if (((PaneledGraph*)graph)->isFlat(i)) continue;
        ed = end = graph->getVertex(i)->getFirstEdge();
        do {
            for (auto clockwise: {true, false}) {
                newRepresentation->setRepresentation(ed, clockwise);
                vector<int> traversedVertices = ((PaneledRepresentation*)newRepresentation)->getTraversedVertices();
                sort(traversedVertices.begin(), traversedVertices.end());
                int checkIndex = -1;
                for (int t = 0; t < existTraversedVertices.size(); ++t) {
                    if (existTraversedVertices[t].size() != traversedVertices.size())
                        continue;
                    bool equivalent = true;
                    for (int ind = 0; ind < existTraversedVertices[t].size(); ++ind) {
                        if (existTraversedVertices[t][ind] != traversedVertices[ind]) {
                            equivalent = false;
                            break;
                        }
                    }
                    if (equivalent) {
                        checkIndex = t;
                        break;
                    }
                }
                if (checkIndex == -1) {
                    existTraversedVertices.push_back(traversedVertices);
                    multipleBestRepresentations.push_back(new PaneledRepresentation(*(PaneledRepresentation*)newRepresentation));
                } else {
                    result = multipleBestRepresentations[checkIndex]->compareRepresentation(*newRepresentation);
                    if (result == Representation::Results::AUTOMORPHISM) {
//                        representations[repsCount++] = new Representation(*newRepresentation);
                    } else if (result == Representation::Results::BETTER) {
//                        repsCount = 0;
//                        representations[repsCount++] = new Representation(*newRepresentation);
                        multipleBestRepresentations[checkIndex] = new PaneledRepresentation(*(PaneledRepresentation*)newRepresentation);
                    }
                }
            }
            ed = ed->getNext();
        } while (ed != end);
    }

    traversedAll = (multipleBestRepresentations.size() == 1);
}

Representation::Results PaneledGraphRepresentation::compareRepresentation(const GraphRepresentation& _graph_rep) const
{
    PaneledGraphRepresentation graphRep = (PaneledGraphRepresentation&)_graph_rep;
    if (multipleBestRepresentations.size() != graphRep.multipleBestRepresentations.size()) return Representation::Results::FAIL;

    Representation::Results result;
    if (multipleBestRepresentations.size() == 1) {
        result = multipleBestRepresentations[0]->compareRepresentation(*graphRep.multipleBestRepresentations[0]);
    } else if (multipleBestRepresentations.size() == 2) {
        result = multipleBestRepresentations[0]->compareRepresentation(*graphRep.multipleBestRepresentations[0]);
        if (result == Representation::Results::AUTOMORPHISM)
            result = multipleBestRepresentations[1]->compareRepresentation(*graphRep.multipleBestRepresentations[1]);

        if (result != Representation::Results::AUTOMORPHISM) {
            result = multipleBestRepresentations[0]->compareRepresentation(*graphRep.multipleBestRepresentations[1]);
            if (result == Representation::Results::AUTOMORPHISM) {
                result = multipleBestRepresentations[1]->compareRepresentation(*graphRep.multipleBestRepresentations[0]);
            }
        }
    } else {
        exit(1);
    }

    return result;
}
