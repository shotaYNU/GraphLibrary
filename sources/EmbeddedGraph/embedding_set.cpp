#include "embedding_set.hpp"

EmbeddingSet::EmbeddingSet()
{

}

EmbeddingSet::~EmbeddingSet()
{

}

void EmbeddingSet::setAllEmbeddings(EmbeddedGraph* _graph)
{
    vector<vector<int>> maps;

    vector<int> newEmbedding;
    vector<long long> adjacentOrigin;
    vector<long long> adjacentMutate;
    vector<pair<int, int>> mapping;

    EmbeddedEdge *ed, *end;
    map<int, int> degs;
    for (int i = 0; i < _graph->getVerticesNum(); ++i) {
        ed = end = _graph->getVertex(i)->getFirstEdge();
        if (degs.count(_graph->getVertex(i)->getDegree()) == 0) {
            degs[_graph->getVertex(i)->getDegree()] = (int)maps.size();
            maps.push_back(vector<int>{i});
        } else {
            maps[degs[_graph->getVertex(i)->getDegree()]].push_back(i);
        }
        adjacentOrigin.push_back(0ULL);
        adjacentMutate.push_back(0ULL);
        newEmbedding.push_back(i);
        do {
            adjacentOrigin.back() += (1ULL << ed->getEnd());
            adjacentMutate.back() += (1ULL << ed->getEnd());
            ed = ed->getNext();
        } while(ed != end);
    }

    vector<vector<int>> originMaps = maps;
    vector<vector<vector<pair<int, int>>>> pairs;
    for (int i = 0; i < maps.size(); ++i) {
        vector<vector<pair<int, int>>> onePairs;
        do {
            vector<pair<int, int>> p = convertToTranspositions(originMaps[i], maps[i]);
            onePairs.push_back(p);
        } while(next_permutation(maps[i].begin(), maps[i].end()));
        pairs.push_back(onePairs);
    }

    vector<int> indexies;
    vector<int> indexiesMax;
    for (int i = 0; i < pairs.size(); ++i) {
        indexies.push_back(0);
        indexiesMax.push_back((int)pairs[i].size());
    }
    vector<int> origin = indexies;
    vector<vector<pair<int, int>>> embCan;
    do {
        vector<pair<int, int>> p;
        for (int i = 0; i < indexies.size(); ++i)
            p.insert(p.end(), pairs[i][indexies[i]].begin(), pairs[i][indexies[i]].end());
        embCan.push_back(p);
        nextIndexies(indexies, indexiesMax);
    } while (origin != indexies);

    long long temp = 0;
    for (int i = 0; i < embCan.size(); ++i) {
        adjacentMutate = adjacentOrigin;
        mapping = embCan[i];
        for (pair<int, int> transposition : mapping) {
            Utility::allExchange(transposition.first, transposition.second, adjacentMutate);
            temp = adjacentMutate[transposition.first];
            adjacentMutate[transposition.first] = adjacentMutate[transposition.second];
            adjacentMutate[transposition.second] = temp;
        }

        if (adjacentOrigin == adjacentMutate)
            embeddings.push_back(mapping);
    }

    setAllInequivalentEmbeddings(_graph);
}

void EmbeddingSet::setAllInequivalentEmbeddings(EmbeddedGraph* _graph)
{
    EmbeddedEdge *ed, *end;
    EmbeddedFace* face;
    vector<EmbeddedFace*> faces;
    vector<long long> originFaces;
    vector<long long> newFaces;
    vector<vector<long long>> existFaces;

    _graph->initFace();
    for (int i = 0; i < _graph->getVerticesNum(); ++i) {
        ed = end = _graph->getVertex(i)->getFirstEdge();
        do {
            face = ed->getNextFace();
            if (find(faces.begin(), faces.end(), face) == faces.end())
                faces.push_back(face);
            ed = ed->getNext();
        } while(ed != end);
    }

    for (auto face : faces) {
        originFaces.push_back(0ULL);
        newFaces.push_back(0ULL);
        for (auto vertex : face->getFace()) {
            originFaces.back() += (1ULL << vertex);
            newFaces.back() += (1ULL << vertex);
        }
    }

    bool exsitAlready = false;
    for (auto embedding : embeddings) {
        newFaces = originFaces;
        for (auto transposition : embedding)
            Utility::allExchange(transposition.first, transposition.second, newFaces);
        sort(newFaces.begin(), newFaces.end());
        exsitAlready = false;
        for (auto compareFaces : existFaces) {
            if (compareFaces == newFaces) {
                exsitAlready = true;
                break;
            }
        }
        if (!exsitAlready) {
            inequivalentEmbeddings.push_back(embedding);
            existFaces.push_back(newFaces);
        }
    }
}

void EmbeddingSet::nextIndexies(vector<int>& indexies, const vector<int> &indexiesMax)
{
    int currentIndex = (int)(indexies.size() - 1);
    while (currentIndex >= 0) {
        indexies[currentIndex] += 1;
        if (indexies[currentIndex] == indexiesMax[currentIndex]) {
            indexies[currentIndex] = 0;
            currentIndex -= 1;
        } else break;
    }
}

vector<pair<int, int>> EmbeddingSet::convertToTranspositions(const vector<int> &_originEmbedding, const vector<int> &_newEmbedding)
{
    map<int, int> indexies;
    for (int i = 0; i < _newEmbedding.size(); ++i) {
        indexies[_originEmbedding[i]] = i;
    }
    int prev = 0;
    int next = 0;
    int start = 0;
    vector<pair<int, int>> mapping;

    for (int i = 0; i < _originEmbedding.size(); ++i) {
        start = prev = _originEmbedding[i];
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

vector<int> EmbeddingSet::convertToPermutation(const vector<pair<int, int>>& _transpositions, int _length)
{
    vector<int> permutation;
    for (int i = 0; i < _length; ++i)
        permutation.push_back(i);

    int temp = 0;
    for (auto transposition : _transpositions) {
        temp = permutation[transposition.first];
        permutation[transposition.first] = permutation[transposition.second];
        permutation[transposition.second] = temp;
    }

    return permutation;
}

vector<string> EmbeddingSet::getSaveDate(int _embeddingLength)
{
    vector<string> data;
    for (auto embedding : inequivalentEmbeddings) {
        vector<int> perm = convertToPermutation(embedding, _embeddingLength);
        string newPermutationStr = "";
        for (auto n : perm) {
            newPermutationStr += get<0>(EmbeddedVertex::idToChar(n, false));
        }
        data.push_back(newPermutationStr);
    }

    return data;
}
