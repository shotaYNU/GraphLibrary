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
    vector<bool*> adjacent;

    for (int i = 0; i < graph->getVerticesNum(); ++i) {
        ed = end = graph->getVertex(i)->getFirstEdge();
        adjacent.push_back(new bool[ADJACENT_MAX]);
        for (int b = 0; b < ADJACENT_MAX; b++)
            adjacent.back()[b] = false;
        int deg = graph->getVertexDegree(i);
        degrees.push_back(make_pair(deg, i));
        do {
            adjacent.back()[ed->getEnd()] = true;
            ed = ed->getNext();
        } while(ed != end);
    }
    sort(degrees.begin(), degrees.end());

    int prev = -1;
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

    array<int, GraphProperty::MAX_EDGE> newAdjacentRepresentation;
    bestAdjacentRepresentation[0] = 10000;
    vector<bool*> newAdjacent;
    for (int i = 0; i < adjacent.size(); ++i) {
        bool* oneAdjacent = new bool[ADJACENT_MAX];
        for (int a = 0; a < ADJACENT_MAX; ++a)
            oneAdjacent[a] = false;
        newAdjacent.push_back(oneAdjacent);
    }
    for (int i = 0; i < pairs.size(); ++i) {
        for (int aindex = 0; aindex < adjacent.size(); ++aindex)
            for (int a = 0; a < ADJACENT_MAX; ++a)
                newAdjacent[aindex][a] = adjacent[aindex][a];
        vector<pair<int, int>> p = Utility::convertToTranspositions(origMap, pairs[i]);
        for (pair<int, int> transposition : p) {
            Utility::allExchangeBool(transposition.first, transposition.second, newAdjacent);
            temp = newAdjacent[transposition.first];
            newAdjacent[transposition.first] = newAdjacent[transposition.second];
            newAdjacent[transposition.second] = temp;
        }
        setAdjacentRepresentation(newAdjacent, newAdjacentRepresentation);
        if (compareAdjacent(bestAdjacentRepresentation, newAdjacentRepresentation) == Results::BETTER)
            for (int r = 0; r < repCount; ++r)
                bestAdjacentRepresentation[r] = newAdjacentRepresentation[r];
    }
}

bool Isomorphism::isomorphic(const Isomorphism& isomorphism) const
{
    if (repCount != isomorphism.repCount) return false;

    if (compareAdjacent(bestAdjacentRepresentation, isomorphism.bestAdjacentRepresentation) == Results::EQUIVALENT)
        return true;

    return false;
}

Isomorphism::Results Isomorphism::compareAdjacent(const array<int, GraphProperty::MAX_EDGE>& _adj1, const array<int, GraphProperty::MAX_EDGE>& _adj2) const
{
    for (int i = 0; i < repCount; ++i) {
        if (_adj1[i] < _adj2[i])
            return Results::FAIL;
        else if (_adj1[i] > _adj2[i])
            return Results::BETTER;
    }

    return Results::EQUIVALENT;
}

void Isomorphism::setAdjacentRepresentation(const vector<bool*>& _adjacent, array<int, GraphProperty::MAX_EDGE>& _newAdjacentRepresentation)
{
    repCount = 0;
    for (auto adj : _adjacent) {
        for (int i = 0; i < _adjacent.size(); ++i)
            _newAdjacentRepresentation[repCount++] = adj[i] ? 1 : 0;
        _newAdjacentRepresentation[repCount++] = 2;
    }
}
