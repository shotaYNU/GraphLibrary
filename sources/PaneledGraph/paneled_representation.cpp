#include "paneled_representation.hpp"

PaneledRepresentation::PaneledRepresentation() : Representation()
{

}

PaneledRepresentation::PaneledRepresentation(const PaneledRepresentation& _obj): Representation(_obj)
{
    for (int i = 0; i < GraphProperty::MAX_EDGE; ++i)
        representationPanel[i] = _obj.representationPanel[i];
}

PaneledRepresentation::~PaneledRepresentation()
{

}

Representation::Results PaneledRepresentation::compareRepresentation(const Representation& _rep) const
{
    PaneledRepresentation panelRep = (PaneledRepresentation&)_rep;
    if (repCount != panelRep.repCount) return Results::FAIL;

    Results panelResult = Results::AUTOMORPHISM;
    for (int i = 0; i < repCount; ++i) {
        if (representation[i] < panelRep.representation[i])
            return Results::FAIL;
        else if (representation[i] > panelRep.representation[i])
            return Results::BETTER;
        else if (panelResult == Results::AUTOMORPHISM) {
            if (representationPanel[i] < panelRep.representationPanel[i])
                panelResult = Results::FAIL;
            else if (representationPanel[i] > panelRep.representationPanel[i])
                panelResult = Results::BETTER;
        }
    }

    return panelResult;
}

void PaneledRepresentation::setRepresentation(EmbeddedEdge* _ed, bool _clockwise)
{
    edge = _ed;
    clockwise = _clockwise;
    traversedVertices.clear();

    PaneledBreadthFirstSearch bfs;
    EmbeddedEdge* nowEdge = nullptr;
    PaneledFace* face = nullptr;
    int nowId = -1;
    int indexCount = 1;
    int vertex = -1;
    int indexMapping[GraphProperty::MAX_VERTEX];
    for (int i = 0; i < GraphProperty::MAX_VERTEX; ++i)
        indexMapping[i] = -1;

    repCount = 0;
    bfs.init(_ed, _clockwise);
    while (bfs.hasNext()) {
        nowEdge = bfs.next();
        if (((PaneledFace*)nowEdge->getNextFace())->getPanel() && ((PaneledFace*)nowEdge->getPrevFace())->getPanel()) continue;
        if (nowId != nowEdge->getStart()) {
            representation[repCount] = 0;
            representationPanel[repCount++] = 0;
            nowId = nowEdge->getStart();
            if (indexMapping[nowId] == -1)
                indexMapping[nowId] = indexCount++;
            traversedVertices.push_back(nowId);
        }
        vertex = nowEdge->getEnd();
        if (indexMapping[vertex] == -1)
            indexMapping[vertex] = indexCount++;

        face = clockwise ? (PaneledFace*)nowEdge->getNextFace() : (PaneledFace*)nowEdge->getPrevFace();
        representation[repCount] = indexMapping[vertex];
        representationPanel[repCount++] = face->getPanel() ? 1 : 2;
    }
    representation[repCount] = 0;
    representationPanel[repCount++] = 0;
}
