#ifndef EmbeddingSet_hpp
#define EmbeddingSet_hpp

#include "embedded_graph.hpp"

class EmbeddingSet {
public:
    //Constructors and a destructor.
    EmbeddingSet();
    ~EmbeddingSet();

    void setAllEmbeddings(EmbeddedGraph* _graph);
    vector<pair<int, int>> getEmbeddings(int _index) const { return embeddings[_index]; }
    int getEmbeddingsNum() { return (int)embeddings.size(); }

protected:
    vector<vector<pair<int, int>>> embeddings;

private:
    vector<pair<int, int>> convertToTranspositions(const vector<int>& _newEmbedding);
    vector<pair<int, int>> convertToTranspositions(const vector<int>& _originEmbedding, const vector<int>& _newEmbedding);
    void nextIndexies(vector<int>& _indexies, const vector<int>& _indexiesMax);

};

#endif
