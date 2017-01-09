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
    bool isFlat(int _id);
    void saveGraph(string _filepath);


protected:
    EmbeddingSet embeddingSet;

private:
    void setPanelsWithAllEmbeddings();
    string toFacesString();

};

#endif
