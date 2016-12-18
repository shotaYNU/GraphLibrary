#include "graph_generate_representation.hpp"

GraphGenerateRepresentation::GraphGenerateRepresentation() : GraphRepresentation()
{

}

GraphGenerateRepresentation::GraphGenerateRepresentation(EmbeddedGraph* _graph): GraphRepresentation(_graph)
{

}

GraphGenerateRepresentation::~GraphGenerateRepresentation()
{
    
}

void GraphGenerateRepresentation::setCandidateLast()
{
    int cl;
    EmbeddedEdge *end, *ed;
    end = ed = graph->getVertex(graph->getVerticesNum() - 1)->getFirstEdge();
    minstart = colors[ed->getStart()];
    maxend = (graphIrreducible || graph->getVertex(ed->getStart())->getCommonCount(graph->getVertex(ed->getEnd())) == 2) ? colors[ed->getEnd()] : 0;

    do {
        if (graphIrreducible || graph->getVertex(ed->getStart())->getCommonCount(graph->getVertex(ed->getEnd())) == 2) {
            if ((cl = colors[ed->getEnd()]) == maxend)
                lastList[lastCount++] = ed;
            else if (cl > maxend) {
                lastCount = 0;
                lastList[lastCount++] = ed;
                maxend = cl;
            }
        }
        ed = ed->getNext();
    } while (ed != end);
}

bool GraphGenerateRepresentation::setCandidateOther()
{
    EmbeddedEdge *ed, *end;
    int cl;

    for (int i = 0; i < graph->getVerticesNum() - 1; ++i) {
        ed = end = graph->getVertex(i)->getFirstEdge();
        do {
            if (graphIrreducible || EmbeddedVertex::getCommonCount(graph->getVertex(ed->getStart()), graph->getVertex(ed->getEnd())) == 2) {
                if ((cl = colors[ed->getStart()]) < minstart) return false;
                if (cl == minstart) {
                    if ((cl = colors[ed->getEnd()]) > maxend) return false;
                    if (cl == maxend) otherList[otherCount++] = ed;
                }
            }
            ed = ed->getNext();
        } while(ed != end);
    }

    return true;
}

bool GraphGenerateRepresentation::setBestRepresentationIfLastBest()
{
    lastCount = otherCount = 0;
    repsCount = 0;
    graphIrreducible = graph->getIrreducible();
    for (int i = 0; i < GraphProperty::MAX_VERTEX; ++i)
        colors[i] = ((MutableGraph*)graph)->getColors()[i];
    setCandidateLast();
    if (!setCandidateOther())
        return false;

    EmbeddedEdge *ed;
    Representation::Results result;

    bestRepresentation->setRepresentation(lastList[0], true);
    for (int i = 0; i < lastCount; ++i) {
        ed = lastList[i];
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
    }

    for (int i = 0; i < otherCount; ++i) {
        ed = otherList[i];
        for (auto clockwise: {true, false}) {
                newRepresentation->setRepresentation(ed, clockwise);
                result = bestRepresentation->compareRepresentation(*newRepresentation);
                if (result == Representation::Results::AUTOMORPHISM) {
                    representations[repsCount++] = new Representation(*newRepresentation);
                } else if (result == Representation::Results::BETTER)
                    return false;
            }
    }

    return true;
}

bool GraphGenerateRepresentation::canEdegMakeBest(EmbeddedEdge* _ed)
{
    Representation rep = Representation();

    rep.setRepresentation(_ed, true);
    if (bestRepresentation->compareRepresentation(rep) == Representation::Results::AUTOMORPHISM)
        return true;

    rep.setRepresentation(_ed, false);
    if (bestRepresentation->compareRepresentation(rep) == Representation::Results::AUTOMORPHISM)
        return true;

    return false;
}
