#include "paneled_isomorphism.hpp"

PaneledIsomorphism::PaneledIsomorphism(PaneledGraph* _graph) : Isomorphism(_graph)
{
}

PaneledIsomorphism::~PaneledIsomorphism()
{
}

void PaneledIsomorphism::setMappedAdjacents()
{
    vector<bool*> adjacent;

//    Start: Set adjacency matrix
    EmbeddedEdge *ed, *end;
    vector<int> realAdjacent;
    for (int i = 0; i < graph->getVerticesNum(); ++i) {
        ed = end = graph->getVertex(i)->getFirstEdge();
        realAdjacent.push_back(i);
        adjacent.push_back(new bool[ADJACENT_MAX]);
        for (int b = 0; b < ADJACENT_MAX; b++)
            adjacent.back()[b] = false;
        do {
            if (((PaneledFace*)ed->getNextFace())->getPanel() && ((PaneledFace*)ed->getPrevFace())->getPanel()) {
                ed = ed->getNext();
                continue;
            }
            adjacent.back()[ed->getEnd()] = true;
            ed = ed->getNext();
        } while(ed != end);
    }
    vector<int> removeList;
    for (int i = 0; i < adjacent.size(); ++i) {
        bool none = true;
        for (int a = 0; a < ADJACENT_MAX; ++a) {
            if (adjacent[i][a]) {
                none = false;
                break;
            }
        }
        if (none)
            removeList.push_back(i);
    }
    reverse(removeList.begin(), removeList.end());
    for (auto index : removeList) {
        adjacent.erase(adjacent.begin() + index);
        for (int i = index; i < adjacent.size(); ++i)
            Utility::allExchangeBool(i, i + 1, adjacent);
        realAdjacent.erase(realAdjacent.begin() + index);
    }
//    End: Set adjacency matrix

//    Start: Set panel
    vector<long long> panel;
    for (int i = 0; i < graph->getVerticesNum(); ++i) {
        ed = end = graph->getVertex(i)->getFirstEdge();
        do {
            if (((PaneledFace*)ed->getNextFace())->getPanel()) {
                vector<int> face = ed->getNextFace()->getFace();
                long long f = 0ULL;
                for (auto v : face)
                    f += (1ULL << v);
                panel.push_back(f);
            }
            ed = ed->getNext();
        } while(ed != end);
    }
    removeList.clear();
    for (int i = 0; i < panel.size(); ++i) {
        for (int j = i + 1; j < panel.size(); ++j) {
            long long set = panel[i] & panel[j];
            int count = 0;
            for (int c = 0; c < ADJACENT_MAX; c++)
                if ((set & (1ULL << c)) != 0)
                    count++;
            if (count >= 3) {
                removeList.push_back(j);
            } else if (count >= 2) {
                removeList.push_back(i);
                removeList.push_back(j);
            }
        }
    }
    sort(removeList.begin(), removeList.end());
    vector<int> lists;
    int prev = -1;
    for (int i = 0; i < removeList.size(); ++i) {
        if (prev != removeList[i]) {
            lists.push_back(removeList[i]);
            prev = removeList[i];
        }
    }
    reverse(lists.begin(), lists.end());
    for (auto index : lists)
        panel.erase(panel.begin() + index);

    vector<long long> oPanel;
    for (auto p : panel) {
        oPanel.push_back(0ULL);
        for (int i = 0; i < ADJACENT_MAX; ++i)
            if ((p & (1ULL << i)) != 0) {
                int index = (int)(find(realAdjacent.begin(), realAdjacent.end(), i) - realAdjacent.begin());
                oPanel.back() += (1ULL << index);
            }
    }
    panel = oPanel;
//    End: Set panel

//    Start: Set disjoint index list
    int id = 0;
    vector<vector<int>> adList;
    vector<int> exists;
    queue<int> remains;
    do {
        for (int i = 0; i < adjacent.size(); ++i)
            if (find(exists.begin(), exists.end(), i) == exists.end()) {
                remains.push(i);
                break;
            }
        vector<int> adIndexies;
        do {
            id = remains.front();
            remains.pop();
            for (int i = 0; i < ADJACENT_MAX; ++i) {
                if (adjacent[id][i] && find(exists.begin(), exists.end(), i) == exists.end()) {
                    adIndexies.push_back(i);
                    exists.push_back(i);
                    remains.push(i);
                }
            }
        } while (!remains.empty());
        sort(adIndexies.begin(), adIndexies.end());
        adList.push_back(adIndexies);
    } while (exists.size() < adjacent.size());
//    End: Set disjoint index list

//    Start: Set disjoint mapped adjacent list
    for (auto ad : adList) {
        vector<bool*> adj;
        for (int i = 0; i < ad.size(); ++i) {
            bool* newAd = new bool[ADJACENT_MAX];
            for (int b = 0; b < ADJACENT_MAX; b++)
                newAd[b] = false;
            for (int j = 0; j < ADJACENT_MAX; ++j)
                if (adjacent[ad[i]][j])
                    newAd[find(ad.begin(), ad.end(), j) - ad.begin()] = true;
            adj.push_back(newAd);
        }
        vector<long long> pan;
        for (auto p : panel) {
            bool fail = false;
            for (int i = 0; i < ADJACENT_MAX; ++i) {
                if ((p & (1ULL << i)) != 0) {
                    if (find(ad.begin(), ad.end(), i) == ad.end()) {
                        fail = true;
                        break;
                    }
                }
            }
            if (!fail)
                pan.push_back(p);
        }
        addAdjacents(adj, pan);
    }
//    End: Set disjoint mapped adjacent list
}

void PaneledIsomorphism::addAdjacents(vector<bool*> _adjacent, vector<long long> _panel)
{
    vector<pair<int, int>> degrees;
    for (int i = 0; i < _adjacent.size(); ++i) {
        int count = 0;
        for (int j = 0; j < ADJACENT_MAX; ++j)
            if (_adjacent[i][j])
                count++;
        degrees.push_back(make_pair(count, i));
    }
    sort(degrees.begin(), degrees.end());

    vector<int> degMap;
    vector<int> origMap;
    vector<vector<int>> maps;
    int prev = -1;
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
        Utility::allExchangeBool(transposition.first, transposition.second, _adjacent);
        temp = _adjacent[transposition.first];
        _adjacent[transposition.first] = _adjacent[transposition.second];
        _adjacent[transposition.second] = temp;
    }

    vector<long long> oPanel;
    for (auto p : _panel) {
        oPanel.push_back(0ULL);
        for (int i = 0; i < ADJACENT_MAX; ++i)
            if ((p & (1ULL << i)) != 0) {
                int index = (int)(find(degMap.begin(), degMap.end(), i) - degMap.begin());
                oPanel.back() += (1ULL << index);
            }
    }
    _panel = oPanel;

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
    array<int, GraphProperty::MAX_EDGE> bestAdjacentRepresentation;
    vector<long long> newPanel = _panel;
    vector<long long> bestPanel = _panel;
    bestAdjacentRepresentation[0] = 10000;
    if (bestPanel.size() != 0)
        bestPanel[0] = (1ULL << 21);
    vector<bool*> newAdjacent;
    for (int i = 0; i < _adjacent.size(); ++i) {
        bool* oneAdjacent = new bool[ADJACENT_MAX];
        for (int a = 0; a < ADJACENT_MAX; ++a)
            oneAdjacent[a] = false;
        newAdjacent.push_back(oneAdjacent);
    }
    for (int i = 0; i < pairs.size(); ++i) {
        for (int aindex = 0; aindex < _adjacent.size(); ++aindex)
            for (int a = 0; a < ADJACENT_MAX; ++a)
                newAdjacent[aindex][a] = _adjacent[aindex][a];
        for (int pindex = 0; pindex < _panel.size(); ++pindex)
            newPanel[pindex] = _panel[pindex];
        vector<pair<int, int>> p = Utility::convertToTranspositions(origMap, pairs[i]);
        for (pair<int, int> transposition : p) {
            Utility::allExchangeBool(transposition.first, transposition.second, newAdjacent);
            temp = newAdjacent[transposition.first];
            newAdjacent[transposition.first] = newAdjacent[transposition.second];
            newAdjacent[transposition.second] = temp;
            Utility::allExchangeLong(transposition.first, transposition.second, newPanel);
        }
        sort(newPanel.begin(), newPanel.end());
        setAdjacentRepresentation(newAdjacent, newAdjacentRepresentation);
        Results result = compareAdjacent(bestAdjacentRepresentation, repCount, newAdjacentRepresentation, repCount);
        if (result == Results::BETTER) {
            for (int r = 0; r < repCount; ++r)
                bestAdjacentRepresentation[r] = newAdjacentRepresentation[r];
            bestPanel = newPanel;
        } else if (result == Results::EQUIVALENT && comparePanel(bestPanel, newPanel) == Results::BETTER) {
            bestPanel = newPanel;
        }
    }

    bestAdjacentRepresentationList.push_back(bestAdjacentRepresentation);
    repCounts.push_back(repCount);
    panelsList.push_back(bestPanel);
}

bool PaneledIsomorphism::isomorphic(const Isomorphism& _isomorphism) const
{
    PaneledIsomorphism isomorphism = (PaneledIsomorphism&)_isomorphism;
    if (bestAdjacentRepresentationList.size() != isomorphism.bestAdjacentRepresentationList.size()) return false;

    if (bestAdjacentRepresentationList.size() == 1) {
        if (isomorphicOne(bestAdjacentRepresentationList[0], panelsList[0], repCounts[0], isomorphism.bestAdjacentRepresentationList[0], isomorphism.panelsList[0], isomorphism.repCounts[0]))
            return true;
    } else if (bestAdjacentRepresentationList.size() == 2) {
        if (isomorphicOne(bestAdjacentRepresentationList[0], panelsList[0], repCounts[0], isomorphism.bestAdjacentRepresentationList[0], isomorphism.panelsList[0], isomorphism.repCounts[0]) &&
            isomorphicOne(bestAdjacentRepresentationList[1], panelsList[1], repCounts[1], isomorphism.bestAdjacentRepresentationList[1], isomorphism.panelsList[1], isomorphism.repCounts[1]))
            return true;
        if (isomorphicOne(bestAdjacentRepresentationList[0], panelsList[0], repCounts[0], isomorphism.bestAdjacentRepresentationList[1], isomorphism.panelsList[1], isomorphism.repCounts[1]) &&
            isomorphicOne(bestAdjacentRepresentationList[1], panelsList[1], repCounts[1], isomorphism.bestAdjacentRepresentationList[0], isomorphism.panelsList[0], isomorphism.repCounts[0]))
            return true;
    } else {
        exit(1);
    }

    return false;
}

bool PaneledIsomorphism::isomorphicOne(const array<int, GraphProperty::MAX_EDGE> &_adj1, const vector<long long> &_panel1, int _repCount1, const array<int, GraphProperty::MAX_EDGE> &_adj2, const vector<long long> &_panel2, int _repCount2) const
{
    if (compareAdjacent(_adj1, _repCount1, _adj2, _repCount2) == Results::EQUIVALENT)
        if (comparePanel(_panel1, _panel2) == Results::EQUIVALENT)
            return true;

    return false;
}

Isomorphism::Results PaneledIsomorphism::compareAdjacent(const array<int, GraphProperty::MAX_EDGE> &_adj1, int _repCount1, const array<int, GraphProperty::MAX_EDGE> &_adj2, int _repCount2) const
{
    if (_repCount1 != _repCount2) return Results::FAIL;
    for (int i = 0; i < _repCount1; ++i) {
        if (_adj1[i] < _adj2[i])
            return Results::FAIL;
        else if (_adj1[i] > _adj2[i])
            return Results::BETTER;
    }

    return Results::EQUIVALENT;
}

Isomorphism::Results PaneledIsomorphism::comparePanel(const vector<long long>& _p1, const vector<long long>& _p2) const
{
    if (_p1.size() != _p2.size()) return Results::FAIL;
    for (int i = 0; i < _p1.size(); ++i) {
        if (_p1[i] < _p2[i])
            return Results::FAIL;
        else if (_p1[i] > _p2[i])
            return Results::BETTER;
    }

    return Results::EQUIVALENT;
}

vector<pair<string, string>> PaneledIsomorphism::toStringDatas()
{
    vector<pair<string, string>> pairs;
    for (int i = 0; i < bestAdjacentRepresentationList.size(); ++i) {
        string repString = "";
        for (int o = 0; o < repCounts[i]; ++o)
            repString += to_string(bestAdjacentRepresentationList[i][o]);
        string panelString = "";
        for (auto onePanel: panelsList[i]) {
            panelString += to_string(onePanel);
            panelString += ",";
        }
        panelString.pop_back();
        pairs.push_back(make_pair(repString, panelString));
    }

    return pairs;
}

void PaneledIsomorphism::setPaneledIsomorphismDatas(const vector<pair<string, string>>& _datas)
{
    for (int i = 0; i < _datas.size(); ++i) {
        string repString = _datas[i].first;
        string panelString = _datas[i].second;
        array<int, GraphProperty::MAX_EDGE> oneRep;
        for (int o = 0; o < repString.size(); ++o)
            oneRep[o] = repString[o] - '0';

        vector<string> panelStrings = Utility::split(panelString, ',');
        vector<long long> onePanel;
        for (int o = 0; o < panelStrings.size(); ++o)
            onePanel.push_back(atoll(panelStrings[o].c_str()));

        bestAdjacentRepresentationList.push_back(oneRep);
        panelsList.push_back(onePanel);
        repCounts.push_back((int)repString.size());
    }
}

void PaneledIsomorphism::saveIsomorphisms(const vector<PaneledGraph*>& _graphs, FileSaveDispatcher& _fileSaveDispatcher)
{
    vector<PaneledIsomorphism> isomorphisms;
    for (auto graph : _graphs) {
        PaneledIsomorphism isomorphism(graph);
        isomorphism.setMappedAdjacents();
        picojson::array embStrings;
        for (int e = 0; e < graph->embeddings.size(); ++e) {
            string embeddingString = "";
            for (auto transposition : graph->embeddings[e]) {
                embeddingString += get<0>(EmbeddedVertex::idToChar(transposition.first, false));
                embeddingString += get<0>(EmbeddedVertex::idToChar(transposition.second, false));
                embeddingString += ",";
            }
            embeddingString.pop_back();
            embStrings.push_back(picojson::value(embeddingString));
        }
        picojson::array isos;
        picojson::array panels;
        vector<pair<string, string>> strings = isomorphism.toStringDatas();
        for (auto str : strings) {
            isos.push_back(picojson::value(str.first));
            panels.push_back(picojson::value(str.second));
        }
        vector<pair<string, picojson::value>> appendDatas{
            make_pair("isomorphismsForIsomorphism", picojson::value(isos)),
            make_pair("panelsForIsomorphism", picojson::value(panels)),
            make_pair("embeddingNum", picojson::value((double)graph->embeddings.size())),
            make_pair("embeddings", picojson::value(embStrings))
        };
        _fileSaveDispatcher.save(graph->toSaveGraph(appendDatas));
    }
}
