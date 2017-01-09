#ifndef PanledBreadthFirstSearch_hpp
#define PanledBreadthFirstSearch_hpp

#include "../GraphOperation/breadth_first_search.hpp"
#include "paneled_face.hpp"

class PaneledBreadthFirstSearch: public BreadthFirstSearch {
public:
    //Constructors and a destructor.
    PaneledBreadthFirstSearch();
    PaneledBreadthFirstSearch(EmbeddedGraph* _graph);
    ~PaneledBreadthFirstSearch();

    //Methods to BreadthFirstSearch
    void init(EmbeddedEdge* _start, bool _clockwise);

protected:

private:

};

#endif
