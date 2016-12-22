#ifndef EmbeddingSet_hpp
#define EmbeddingSet_hpp

#include "embedded_graph.hpp"

class EmbeddingSet {
public:
    //Constructors and a destructor.
    EmbeddingSet();
    ~EmbeddingSet();

    void setAllEmbeddings(EmbeddedGraph* _graph);

protected:
    vector<vector<pair<int, int>>> embeddings;

private:
    vector<pair<int, int>> convertToTranspositions(const vector<int>& _newEmbedding);

};

#endif
