#include "embedding_set.hpp"

EmbeddingSet::EmbeddingSet()
{
    
}

EmbeddingSet::~EmbeddingSet()
{
    
}

void EmbeddingSet::setAllEmbeddings(EmbeddedGraph* _graph)
{
    vector<int> newEmbedding;
    vector<long long> adjacentOrigin;
    vector<long long> adjacentMutate;
    vector<pair<int, int>> mapping;

    EmbeddedEdge *ed, *end;
    for (int i = 0; i < _graph->getVerticesNum(); ++i) {
        ed = end = _graph->getVertex(i)->getFirstEdge();
        adjacentOrigin.push_back(0ULL);
        adjacentMutate.push_back(0ULL);
        newEmbedding.push_back(i);
        do {
            adjacentOrigin.back() += (1ULL << ed->getEnd());
            adjacentMutate.back() += (1ULL << ed->getEnd());
            ed = ed->getNext();
        } while(ed != end);
    }

    long long temp = 0;
    do {
        adjacentMutate = adjacentOrigin;
        mapping = convertToTranspositions(newEmbedding);
        for (pair<int, int> transposition : mapping) {
            Utility::allExchange(transposition.first, transposition.second, adjacentMutate);
            temp = adjacentMutate[transposition.first];
            adjacentMutate[transposition.first] = adjacentMutate[transposition.second];
            adjacentMutate[transposition.second] = temp;
        }

        if (adjacentOrigin == adjacentMutate)
            embeddings.push_back(mapping);
    } while (next_permutation(newEmbedding.begin(), newEmbedding.end()));
}

vector<pair<int, int>> EmbeddingSet::convertToTranspositions(const vector<int>& _newEmbedding)
{
    map<int, int> indexies;
    vector<int> originEmbedding;
    for (int i = 0; i < _newEmbedding.size(); ++i) {
        originEmbedding.push_back(i);
        indexies[originEmbedding[i]] = i;
    }
    int prev = 0;
    int next = 0;
    int start = 0;
    vector<pair<int, int>> mapping;

    for (int i = 0; i < originEmbedding.size(); ++i) {
        start = prev = originEmbedding[i];
        next = _newEmbedding[i];
        if (indexies[next] == -1) continue;
        while (start != next) {
            mapping.push_back(make_pair(prev, next));
            prev = next;
            next = _newEmbedding[indexies[prev]];
            indexies[prev] = -1;
        }
        indexies[next] = -1;
    }

    return mapping;
}
