#include "embedded_vertex.hpp"

EmbeddedVertex::EmbeddedVertex()
{
    id = -1;
    degree = 0;
    firstEdge = nullptr;
    isContractible = false;
    contractibleEdgesNum = 0;
    initAdjacent();
}

EmbeddedVertex::EmbeddedVertex(int _id)
{
    id = _id;
    degree = 0;
    firstEdge = nullptr;
    isContractible = false;
    contractibleEdgesNum = 0;
    initAdjacent();
}

EmbeddedVertex::~EmbeddedVertex()
{
    EmbeddedEdge *ed, *next, *end;
    ed = end = next = firstEdge;

    do {
        ed = next;
        next = ed->getNext();
        if (ed != nullptr) {
            if (ed->getOpposite() != nullptr) {
                delete ed->getOpposite();
                ed->setOpposite(nullptr);
            }

            delete ed;
            ed = nullptr;
        }
    } while (next != end);
}

void EmbeddedVertex::initAdjacent()
{
    for(int i = 0; i < GraphProperty::MAX_VERTEX; ++i)
        adjacent[i] = false;
}

void EmbeddedVertex::setId(int _id)
{
    EmbeddedEdge *ed, *end;
    ed = end = firstEdge;

    do {
        ed->setStart(_id);
        ed->getInverse()->setEnd(_id);
        ed = ed->getNext();
    } while (ed != end);

    id = _id;
}

//Convert int data to char data.
//Examples: 0 -> a, 5 -> f, 26 -> A, 32 -> G
tuple<char, bool> EmbeddedVertex::idToChar(int _id, bool _flip)
{
    tuple<char, bool> idValue;

    if (!_flip) {
        if (_id >= 0 && _id < 26) { get<0>(idValue) = (_id + 'a'); get<1>(idValue) = false; }
        else if (_id >= 26 && _id < 52) { get<0>(idValue) = (_id - 26 + 'a'); get<1>(idValue) = true; }
    } else {
        if (_id >= 0 && _id < 26) { get<0>(idValue) = (_id + 'A'); get<1>(idValue) = false; }
        else if (_id >= 26 && _id < 52) { get<0>(idValue) = (_id - 26 + 'A'); get<1>(idValue) = true; }
    }

    return idValue;
}

//Convert char data to int data.
//Examples: a -> 0, f -> 5, A -> 26, G -> 32
tuple<int, bool> EmbeddedVertex::charToId(int _ch)
{
    tuple<int, bool> ids;

    if (_ch >= 'a' && _ch <= 'z') { get<0>(ids) = (_ch - 'a'); get<1>(ids) = false; }
    if (_ch >= 'A' && _ch <= 'Z') { get<0>(ids) = (_ch - 'A'); get<1>(ids) = true; }

    return ids;
}

int EmbeddedVertex::getCommonCount(const EmbeddedVertex* _vtx) const
{
    int count = 0;

    for (int i = 0; i < GraphProperty::MAX_VERTEX; ++i) {
        if (adjacent[i] && _vtx->adjacent[i])
            count++;
    }

    return count;
}

int EmbeddedVertex::getCommonCount(const EmbeddedVertex* _vtx1, const EmbeddedVertex* _vtx2)
{
    int count = 0;

    for (int i = 0; i < GraphProperty::MAX_VERTEX; ++i) {
        if (_vtx1->adjacent[i] && _vtx2->adjacent[i])
            count++;
    }

    return count;
}
