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
    vector<pair<int, int>> getInequivalentEmbeddings(int _index) const { return inequivalentEmbeddings[_index]; }
    int getInequivalentEmbeddingsNum() { return (int)inequivalentEmbeddings.size(); }
    string getSaveData(int _index, int _embeddingLength);
    vector<string> getSaveDatas(int _embeddingLength);

protected:
    vector<vector<pair<int, int>>> embeddings;
    vector<vector<pair<int, int>>> inequivalentEmbeddings;

private:
    void setAllInequivalentEmbeddings(EmbeddedGraph* _graph);

};

#endif
