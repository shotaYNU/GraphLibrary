#include "representation.hpp"

Representation::Representation()
{
    repCount = 0;
    edge = nullptr;
    clockwise = false;
}

Representation::Representation(const Representation& _obj)
{
    for (int i = 0; i < GraphProperty::MAX_EDGE; ++i)
        representation[i] = _obj.representation[i];

    repCount = _obj.repCount;
    edge = _obj.edge;
    clockwise = _obj.clockwise;
}

Representation::~Representation()
{

}

Representation::Results Representation::compareRepresentation(const Representation& _rep) const
{
    if (repCount != _rep.repCount) return Results::FAIL;

    for (int i = 0; i < repCount; ++i) {
        if (representation[i] < _rep.representation[i])
            return Results::FAIL;
        else if (representation[i] > _rep.representation[i])
            return Results::BETTER;
    }

    return Results::AUTOMORPHISM;
}

string Representation::toString() const
{
    string str = "";

    for (int i = 0; i < repCount; ++i)
        str += (to_string(representation[i]) + " ");

    return str;
}

void Representation::setRepresentation(EmbeddedEdge* _ed, bool _clockwise)
{
    edge = _ed;
    clockwise = _clockwise;

    BreadthFirstSearch bfs;
    EmbeddedEdge* nowEdge = nullptr;
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
        if (nowId != nowEdge->getStart()) {
            representation[repCount++] = 0;
            nowId = nowEdge->getStart();
            if (indexMapping[nowId] == -1)
                indexMapping[nowId] = indexCount++;
        }
        vertex = nowEdge->getEnd();
        if (indexMapping[vertex] == -1)
            indexMapping[vertex] = indexCount++;

        representation[repCount++] = indexMapping[vertex];
    }
    representation[repCount++] = 0;
}
