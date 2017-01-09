#ifndef BreadthFirstSearch_hpp
#define BreadthFirstSearch_hpp

#include "../EmbeddedGraph/embedded_graph.hpp"

class BreadthFirstSearch {
public:
    //Constructors and a destructor.
    BreadthFirstSearch();
    BreadthFirstSearch(EmbeddedGraph* _graph);
    ~BreadthFirstSearch();

    //Methods to BreadthFirstSearch
    void init(EmbeddedEdge* _start, bool _clockwise);
    bool hasNext() const { return nowCount != endCount; }
    EmbeddedEdge* next() { return order[nowCount++]; }

protected:
    EmbeddedGraph* searchGraph;
    EmbeddedEdge* start;
    EmbeddedEdge* waiting[GraphProperty::MAX_EDGE];
    EmbeddedEdge* order[GraphProperty::MAX_EDGE];
    bool visited[GraphProperty::MAX_VERTEX] = { false };
    bool clockwise;
    int endCount, nowCount;

private:

};

#endif
