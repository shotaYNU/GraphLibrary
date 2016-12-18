#include "mutable_graph.hpp"

MutableGraph::MutableGraph() : EmbeddedGraph()
{
    for (int i = 0; i < 2 * EDGE_MAX_SIZE; ++i)
        edges[i] = new EmbeddedEdge();
}

MutableGraph::~MutableGraph()
{
    /*for (int i = 0; i < 2 * EDGE_MAX_SIZE; ++i) {
        if (edges[i] != nullptr) {
            delete edges[i];
            edges[i] = nullptr;
        }
    }*/
}

EmbeddedEdge** MutableGraph::getEdgeAddressDegn(int _vtxNum)
{
    if (_vtxNum == 3)
        return edges + GraphProperty::MAX_EDGE + (_vtxNum << 3);
    else if (_vtxNum == 4)
        return edges + GraphProperty::MAX_EDGE + 8 * GraphProperty::MAX_VERTEX + (_vtxNum << 3);
    else
        return edges + GraphProperty::MAX_EDGE + 16 * GraphProperty::MAX_VERTEX + (_vtxNum << 7);
}

/*****   Make a vertex of degree n.   *****/
EmbeddedVertex* MutableGraph::makeVertexDegn(int _n)
{
    int newId = verticesNum;
    EmbeddedEdge** nowEdgeAddress;
    nowEdgeAddress = getEdgeAddressDegn(verticesNum);
    EmbeddedEdge *firstEdge, *prevEdge, *newEdge;
    bool isFirst = true;

    addEdgeAddress = nowEdgeAddress;
    for (int i = 0; i < _n; ++i) {
        newEdge = makeEdge(newId, -1, false);
        if (isFirst) {
            addVertex();
            firstEdge = newEdge;
            vertices[newId]->setFirstEdge(newEdge);
            vertices[newId]->setDegree(_n);
            isFirst = false;
        } else {
            prevEdge->setNext(newEdge);
            newEdge->setPrev(prevEdge);
        }
        prevEdge = newEdge;
    }

    prevEdge->setNext(firstEdge);
    firstEdge->setPrev(prevEdge);

    return vertices[newId];
}

/*****   Delete 2 * (n - 3) edges and add a vertex of degree n for a split.   *****/
void MutableGraph::splitDegn(EmbeddedEdge* _ed, int _n, EmbeddedEdge** _savelist)
{
    /*****   Make a vertex of degree n.   *****/
    EmbeddedVertex* newVertex = makeVertexDegn(_n);

    /*****   Set delete edge.   *****/
    removeEdges(_ed, _n - 3, _savelist);

    /*****   Set a new vertex.   *****/
    EmbeddedEdge *now, *end, *addEdge;
    int setId;

    now = end = newVertex->getFirstEdge();
    do {
        addEdge = now->getInverse();
        setId = _ed->getStart();
        now->setEnd(setId);
        addEdge->setStart(setId);
        newVertex->setAdjacent(setId);
        insertEdge(_ed, addEdge);

        now = now->getNext();
        _ed = _ed->getInverse()->getPrev();
    } while (now != end);
}

/*****   Delete 2 * n edges and delete a vertex of degree n for a contraction.   *****/
void MutableGraph::contractDegn(EmbeddedEdge* _ed, int _n, EmbeddedEdge** _savelist)
{
    int deleteId = _ed->getNext()->getEnd();
    EmbeddedEdge* nextEdge = _ed;
    EmbeddedEdge *otherEdge, *startEdge;

    /* remove edges */
    EmbeddedEdge *start, *end;
    start = end = vertices[deleteId]->getFirstEdge();
    do {
        removeEdge(start->getInverse());
        start = start->getNext();
    } while (start != end);

    /* set new edges */
    vector<EmbeddedEdge*> inserts;
    otherEdge = _ed->getInverse()->getPrev();
    for (int i = 0; i < 2 * (_n - 3); i += 2) {
        otherEdge = startEdge = otherEdge->getInverse()->getPrev();

        _savelist[i]->setStart(nextEdge->getStart());
        _savelist[i]->setEnd(otherEdge->getStart());
        _savelist[i + 1]->setStart(otherEdge->getStart());
        _savelist[i + 1]->setEnd(nextEdge->getStart());

        inserts.push_back(_savelist[i]);
        insertEdge(startEdge, _savelist[i + 1]);
        edgesNum += 2;
    }

    insertEdges(_ed, inserts);

    /*****   Delete an vertex.   *****/
    deleteVertex();
}

tuple<int, int> MutableGraph::getContractibleMinDegree()
{
    int minDeg = 100;
    int minDegNum = 0;

    for (int i = 0; i < verticesNum; ++i) {
        if (!vertices[i]->getIsContractible()) continue;

        if (minDeg == vertices[i]->getDegree())
            minDegNum++;
        else if (minDeg > vertices[i]->getDegree()) {
            minDeg = vertices[i]->getDegree();
            minDegNum = 1;
        }
    }

    return make_tuple(minDeg, minDegNum);
}

void MutableGraph::unMarkAllEdge()
{
    EmbeddedEdge *ed, *end;

    for (int i = 0; i < verticesNum; ++i) {
        ed = end = vertices[i]->getFirstEdge();
        do {
            ed->unmark();
            ed = ed->getNext();
        } while(ed != end);
    }
}

int MutableGraph::makeColors(EmbeddedEdge* _ed)
{
    int c, c0, nc;
    EmbeddedEdge* ed;
    int v1, v2, v3;
    int deg[GraphProperty::MAX_VERTEX];

    for (int i = 0; i < verticesNum; ++i)
        deg[i] = vertices[i]->getDegree();

    v1 = _ed->getStart();
    v2 = _ed->getEnd();
    v3 = _ed->getNext()->getEnd();

    c0 = (1 << ((++deg[v1]) & 7)) + (1 << ((++deg[v2]) & 7)) + (1 << ((++deg[v3]) & 7));

    nc = 1;

    for (int i = verticesNum; --i >= 0;) {
        if (deg[i] == 3) {
            ed = vertices[i]->getFirstEdge();
            c = (1 << ((deg[ed->getEnd()]) & 7)) + (1 << ((deg[ed->getNext()->getEnd()]) & 7)) + (1 << ((deg[ed->getNext()->getNext()->getEnd()]) & 7));

            if (c > c0) {
                --deg[v1];
                --deg[v2];
                --deg[v3];
                return 0;
            } else if (c == c0)
                ++nc;
        }
    }

    --deg[v1];
    --deg[v2];
    --deg[v3];

    return nc;
}
