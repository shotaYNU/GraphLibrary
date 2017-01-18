#include "graph_generation.hpp"

GraphGeneration::GraphGeneration()
{
    generatedNum = 0;
}

GraphGeneration::~GraphGeneration()
{
    delete spList;
}

int GraphGeneration::generateStart(MutableGraph *_graph)
{
    graph = _graph;
    spList = new SplitList(graph);
    graphRepresentation.init(graph);
    graphRepresentation.setBestRepresentation();
    aut.setEdgeAutohomeomorphism(&graphRepresentation);
    aut.setAllAutohomeomorphism();
    doSplittingLoop();

    return generatedNum;
}

void GraphGeneration::doSplittingLoop()
{
    if (isContinue()) {
        continueAction();
    } else {
        terminateAction();
        return;
    }

    SplitList spList(graph);
    spList.setSplitListAll(&aut);

    EmbeddedEdge* splitEdge;
    int deg;
    EmbeddedEdge* saveList[GraphProperty::MAX_EDGE];
    EmbeddedEdge *ed1, *ed2;
    int nc;

    for (int c = 0; c < spList.getSplitCount(); ++c) {
        deg = spList.getSplitDeg(c);
        splitEdge = spList.getSplitEdge(c);
        if (deg == 3) {
            nc = graph->makeColors(splitEdge);
            if (nc > 0) {
                graph->splitDegn(splitEdge, deg, saveList);
                if (nc == 1 && !isContinue()) {
                    terminateAction();
                }
                else if (graphRepresentation.setBestRepresentationIfLastBest()) {
                    aut.setEdgeAutohomeomorphism(&graphRepresentation);
                    aut.setAllAutohomeomorphism();
                    doSplittingLoop();
                }
                graph->contractDegn(splitEdge, deg, saveList);
            }
        } else {
            graph->splitDegn(splitEdge, deg, saveList);
            graph->setColors();
            if (graphRepresentation.setBestRepresentationIfLastBest()) {
                if (deg == 4) {
                    ed1 = splitEdge->getNext()->getInverse();
                    ed2 = ed1->getNext()->getNext();
                    if (graphRepresentation.canEdegMakeBest(ed1) || graphRepresentation.canEdegMakeBest(ed2)) {
                        aut.setEdgeAutohomeomorphism(&graphRepresentation);
                        aut.setAllAutohomeomorphism();
                        doSplittingLoop();
                    }
                } else {
                    ed1 = splitEdge->getNext()->getInverse();
                    if (graphRepresentation.canEdegMakeBest(ed1)) {
                        aut.setEdgeAutohomeomorphism(&graphRepresentation);
                        aut.setAllAutohomeomorphism();
                        doSplittingLoop();
                    }
                }
            }

            graph->contractDegn(splitEdge, deg, saveList);
        }
    }
}
