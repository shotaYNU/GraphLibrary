#ifndef PaneledGraph_hpp
#define PaneledGraph_hpp

#include "../EmbeddedGraph/embedded_graph.hpp"
#include "paneled_face.hpp"
#include "embedding_set.hpp"

class PaneledGraph : public EmbeddedGraph {
public:
    //A constructor and a destructor.
    PaneledGraph();
    ~PaneledGraph();

    //Operation of panel.
    bool isPIT();


protected:
    EmbeddingSet embeddingSet;

private:
    bool isFlat(int _id);
    void setPanelsWithAllEmbeddings();

};

#endif
