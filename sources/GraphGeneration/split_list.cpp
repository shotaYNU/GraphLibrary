#include "split_list.hpp"
//#define VIEW_EXT

SplitList::SplitList(MutableGraph* _graph)
{
    graph = _graph;
    splitCount = 0;
    minDegNum = 0;
    minDeg = 0;
    total = 0;
    mirrorStart = 0;
}

SplitList::~SplitList()
{
    
}

void SplitList::setSplitListDeg3()
{
    EmbeddedEdge *ed;
    int end;

    for (int i = 0; i < graph->getEdgesNum(); ++i) {
        ed = aut->getAutosEdge(0, i);

        if (ed->getMark()) continue;

        end = (mirrorStart == 0) ? total : mirrorStart;
        splitEdgeList[splitCount] = ed;
        splitDegList[splitCount++] = 3;

        markAllMappingEdge(true, ed, 0, end, 3);
        markAllMappingEdge(false, ed, end, total, 3);

        ed = ed->getInverse()->getPrev();
        markAllMappingEdge(true, ed, 0, end, 3);
        markAllMappingEdge(false, ed, end, total, 3);

        ed = ed->getInverse()->getPrev();
        markAllMappingEdge(true, ed, 0, end, 3);
        markAllMappingEdge(false, ed, end, total, 3);
    }
}

void SplitList::setSplitListDeg4()
{
    EmbeddedEdge *ed, *edAnother;
    int end;

    for (int i = 0; i < graph->getEdgesNum(); ++i) {
        ed = aut->getAutosEdge(0, i);
        edAnother = ed->getNext()->getInverse()->getPrev();

        if (ed->getMark()) continue;
        if (minDeg == 3 && (graph->getVertexDegree(ed->getEnd()) == 3) + (graph->getVertexDegree(edAnother->getEnd()) == 3) != minDegNum) continue;

        end = (mirrorStart == 0) ? total : mirrorStart;
        splitEdgeList[splitCount] = ed;
        splitDegList[splitCount++] = 4;

        markAllMappingEdge(true, ed, 0, end, 4);
        markAllMappingEdge(false, ed, end, total, 4);
        markAllMappingEdge(true, edAnother, 0, end, 4);
        markAllMappingEdge(false, edAnother, end, total, 4);
    }
}

void SplitList::setSplitListDegn(int _n)
{
    EmbeddedEdge *ed;
    int end;

    for (int i = 0; i < graph->getEdgesNum(); ++i) {
        ed = aut->getAutosEdge(0, i);

        if (ed->getMark()) continue;
        if (graph->getVertexDegree(ed->getStart()) < 2 * _n - 4) continue;

        end = (mirrorStart == 0) ? total : mirrorStart;
        splitEdgeList[splitCount] = ed;
        splitDegList[splitCount++] = _n;

        markAllMappingEdge(true, ed, 0, end, _n);
        markAllMappingEdge(false, ed, end, total, _n);
    }
}

void SplitList::setSplitListDegnPlus()
{
    EmbeddedEdge *ed, *edAnother;
    int end;

    for (int i = 0; i < graph->getEdgesNum(); ++i) {
        ed = aut->getAutosEdge(0, i);

        if (ed->getMark()) continue;
        if (graph->getVertexDegree(ed->getStart()) < 2 * (minDeg + 1) - 4) continue;

        edAnother = ed;
        for (int j = 0; j < (minDeg + 1) - 2; ++j)
            edAnother = edAnother->getNext();

        if ((graph->getVertexDegree(ed->getEnd()) == minDeg && graph->getVertex(ed->getEnd())->getIsContractible())
                + (graph->getVertexDegree(edAnother->getEnd()) == minDeg && graph->getVertex(edAnother->getEnd())->getIsContractible()) != minDegNum) continue;

        end = (mirrorStart == 0) ? total : mirrorStart;
        splitEdgeList[splitCount] = ed;
        splitDegList[splitCount++] = minDeg + 1;

        markAllMappingEdge(true, ed, 0, end, minDeg + 1);
        markAllMappingEdge(false, ed, end, total, minDeg + 1);
    }
}

void SplitList::markAllMappingEdge(bool _clockwise, EmbeddedEdge* _ed, int _start, int _end, int _deg)
{
    EmbeddedEdge *mappedEdge, *mappedEdgeOp;
    int index = _ed->getIndex();

    if (index >= 0) {
        for (int i = _start; i < _end; ++i) {
            mappedEdge = _ed = aut->getAutosEdge(i, index);
            mappedEdgeOp = mappedEdge->getOpposite();

            if (_deg == 3) {
                if (!_clockwise) mappedEdge = mappedEdge->getInverse();
                else mappedEdgeOp = mappedEdgeOp->getInverse();
            } else {
                if (!_clockwise) {
                    for (int nx = 0; nx < _deg - 2; ++nx)
                        mappedEdge = mappedEdge->getPrev();
                } else {
                    for (int nx = 0; nx < _deg - 2; ++nx)
                        mappedEdgeOp = mappedEdgeOp->getPrev();
                }
            }

            mappedEdge->mark();
            mappedEdgeOp->mark();
        }
    } else {
        index = _ed->getOpposite()->getIndex();
        for (int i = _start; i < _end; ++i) {
            mappedEdge = aut->getAutosEdge(i, index);
            mappedEdgeOp = mappedEdge->getOpposite();

            if (_deg == 3) {
                if (!_clockwise) mappedEdgeOp = mappedEdgeOp->getInverse();
                else mappedEdge = mappedEdge->getInverse();
            } else {
                if (!_clockwise) {
                    for (int nx = 0; nx < _deg - 2; ++nx)
                        mappedEdgeOp = mappedEdgeOp->getPrev();
                } else {
                    for (int nx = 0; nx < _deg - 2; ++nx)
                        mappedEdge = mappedEdge->getPrev();
                }
            }

            mappedEdge->mark();
            mappedEdgeOp->mark();
        }
    }
}

void SplitList::init(Automorphism* _aut)
{
    aut = _aut;
    mirrorStart = _aut->getNormalNum();
    total = _aut->getNormalNum() + _aut->getMirrorNum();

    for (int i = 0; i < graph->getEdgesNum(); ++i) {
        _aut->getAutosEdge(0, i)->setIndex(i);
    }
}

void SplitList::setSplitListAll(Automorphism* _aut)
{
    init(_aut);
    graph->initContractible();
    tuple<int, int> min = graph->getContractibleMinDegree();
    minDeg = get<0>(min);
    minDegNum = get<1>(min);

    //Generate for min degree 3.
    graph->unMarkAllEdge();
    setSplitListDeg3();

#ifdef VIEW_EXT
    cout << "ext3: " << splitCount << endl;
    int count = splitCount;
#endif

    //Generate for min degree 4.
    if (minDeg > 3 || (minDeg == 3 && minDegNum <= 2)) {
        graph->unMarkAllEdge();
        setSplitListDeg4();
    }

#ifdef VIEW_EXT
    cout << "ext4: " << splitCount - count << endl;
    count = splitCount;
#endif

    //Generate for min degree (5 ~ mindeg).
    for (int i = 5; i <= minDeg; ++i) {
        graph->unMarkAllEdge();
        setSplitListDegn(i);
    }

    //Generate for min degree (mindeg + 1).
    if (minDeg >= 4 && minDegNum <= 2) {
        graph->unMarkAllEdge();
        setSplitListDegnPlus();
    }

#ifdef VIEW_EXT
    cout << "extn: " << splitCount - count << endl << endl;
#endif
}
