#ifndef PaneledGraph_hpp
#define PaneledGraph_hpp

#include "../EmbeddedGraph/embedded_graph.hpp"
#include "paneled_face.hpp"

class PaneledGraph : public EmbeddedGraph {
public:
    //A constructor and a destructor.
    PaneledGraph();
    ~PaneledGraph();

    //Operation of panel.
    bool isPIT();

protected:

private:
    bool isFlat(int _id);

};

#endif
