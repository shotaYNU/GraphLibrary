#include "paneled_breadth_first_search.hpp"

PaneledBreadthFirstSearch::PaneledBreadthFirstSearch(): BreadthFirstSearch()
{

}

PaneledBreadthFirstSearch::PaneledBreadthFirstSearch(EmbeddedGraph* _graph): BreadthFirstSearch(_graph)
{

}

PaneledBreadthFirstSearch::~PaneledBreadthFirstSearch()
{

}

void PaneledBreadthFirstSearch::init(EmbeddedEdge* _start, bool _clockwise)
{
    start = _start;
    clockwise = _clockwise;
    for (int i = 0; i < GraphProperty::MAX_VERTEX; ++i)
        visited[i] = false;

    int waitingRealCount = 0;
    int waitnigNowCount = 0;
    int count = 0;
    EmbeddedEdge *now, *end;

    waiting[waitingRealCount++] = start;
    while (waitingRealCount != waitnigNowCount) {
        now = end = waiting[waitnigNowCount++];
        visited[now->getStart()] = true;

        do {
            if (((PaneledFace*)now->getNextFace())->getPanel() && ((PaneledFace*)now->getPrevFace())->getPanel()) {
                now = (clockwise) ? now->getNext() : now->getPrev();
                continue;
            }
            if (!visited[now->getEnd()]) {
                waiting[waitingRealCount++] = now->getInverse();
                visited[now->getEnd()] = true;
            }
            order[count++] = now;
            now = (clockwise) ? now->getNext() : now->getPrev();
        } while (now != end);
    }

    endCount = count;
    nowCount = 0;
}
