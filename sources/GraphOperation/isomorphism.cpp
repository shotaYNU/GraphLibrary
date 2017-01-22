#include "isomorphism.hpp"

Isomorphism::Isomorphism(EmbeddedGraph* _graph)
{
    graph = _graph;
}

Isomorphism::~Isomorphism()
{
}

void Isomorphism::setMappedAdjacents()
{
    vector<vector<int>> maps;
    vector<pair<int, int>> mapping;
    EmbeddedEdge *ed, *end;
    vector<pair<int, int>> degrees;

    for (int i = 0; i < graph->getVerticesNum(); ++i) {
        ed = end = graph->getVertex(i)->getFirstEdge();
        adjacent.push_back(new bool[20]);
        for (int b = 0; b < 20; b++)
            adjacent.back()[b] = false;
        int deg = graph->getVertexDegree(i);
        degrees.push_back(make_pair(deg, i));
        do {
            adjacent.back()[ed->getEnd()] = true;
            ed = ed->getNext();
        } while(ed != end);
    }
    sort(degrees.begin(), degrees.end());
    int nowColor = 0;
    int prev = -1;
    for (int i = 0; i < degrees.size(); ++i) {
        if (prev != degrees[i].first) {
            prev = degrees[i].first;
            degrees[i].first = ++nowColor;
        } else {
            degrees[i].first = nowColor;
        }
    }

    vector<int> degMap;
    vector<int> origMap;

    for (int i = 0; i < degrees.size(); ++i) {
        origMap.push_back(i);
        degMap.push_back(degrees[i].second);
        if (prev != degrees[i].first) {
            maps.push_back(vector<int>{i});
            prev = degrees[i].first;
        } else {
            maps.back().push_back(i);
        }
    }
    vector<pair<int, int>> p = Utility::convertToTranspositions(origMap, degMap);
    bool* temp;
    for (pair<int, int> transposition : p) {
        Utility::allExchangeBool(transposition.first, transposition.second, adjacent);
        temp = adjacent[transposition.first];
        adjacent[transposition.first] = adjacent[transposition.second];
        adjacent[transposition.second] = temp;
    }

    vector<vector<int>> originMaps = maps;
    vector<vector<int>> pairs;
    vector<vector<int>> prevPairs;
    for (int i = 0; i < maps.size(); ++i) {
        vector<vector<pair<int, int>>> onePairs;
        pairs.clear();
        do {
            for (auto pair : prevPairs) {
                vector<int> p = pair;
                p.insert(p.end(), maps[i].begin(), maps[i].end());
                pairs.push_back(p);
            }
            if (prevPairs.empty())
                pairs.push_back(maps[i]);
        } while(next_permutation(maps[i].begin(), maps[i].end()));
        prevPairs = pairs;
    }

    for (int i = 0; i < pairs.size(); ++i) {
        vector<bool*> newAjacent = adjacent;
        vector<pair<int, int>> p = Utility::convertToTranspositions(origMap, pairs[i]);
        for (pair<int, int> transposition : p)
            Utility::allExchangeBool(transposition.first, transposition.second, newAjacent);
        mappedAdjacents.push_back(newAjacent);
    }
}

bool Isomorphism::isomorphic(const Isomorphism& isomorphism) const
{
    if (mappedAdjacents.size() != isomorphism.mappedAdjacents.size()) return false;

    for (auto adj : mappedAdjacents)
        if (equal(adj, isomorphism.adjacent))
            return true;

    return false;
}

bool Isomorphism::equal(const vector<bool*>& _adj1, const vector<bool*>& _adj2) const
{
    if (_adj1.size() != _adj2.size()) return false;

    for (int i = 0; i < _adj1.size(); ++i)
        for (int j = 0; j < _adj1.size(); ++j)
            if (_adj1[i][j] != _adj2[i][j])
                return false;

    return true;
}
