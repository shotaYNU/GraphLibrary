#ifndef ReEmbeddingPairGeneration_hpp
#define ReEmbeddingPairGeneration_hpp

#include "paneled_graph.hpp"
#include "paneled_graph_representation.hpp"
#include "embedding_set.hpp"

class ReEmbeddingPairGeneration {
public:
    ReEmbeddingPairGeneration();
    ~ReEmbeddingPairGeneration();

    void generate(PaneledGraph* _graph);
    int getPairsNum() { return (int)graphRepresentations.size(); }

protected:

private:
    vector<PaneledGraphRepresentation> graphRepresentations;

    bool isFullPanel(const vector<PaneledFace*>& _faces);
    bool contains(const PaneledGraphRepresentation& _newGraphRepresentation);

};

#endif
