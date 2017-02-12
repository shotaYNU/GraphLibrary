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
    vector<vector<int>> panel;
    for (int i = 0; i < graph->getVerticesNum(); ++i) {
        ed = end = graph->getVertex(i)->getFirstEdge();
        do {
            if (((PaneledFace*)ed->getNextFace())->getPanel()) {
                vector<int> face = ed->getNextFace()->getFace();
                if (find(panel.begin(), panel.end(), face) == panel.end())
                    panel.push_back(face);
            }
            ed = ed->getNext();
        } while(ed != end);
    }

//    Set concate faces
    panel = getConvertedFaces(panel);
    
//    Edit panels since there may be empty verteices.
    vector<vector<int>> oPanel;
    for (auto p : panel) {
        vector<int> newPanel;
        for (auto v : p) {
            int index = (int)(find(realAdjacent.begin(), realAdjacent.end(), v) - realAdjacent.begin());
            newPanel.push_back(index);
        }
        oPanel.push_back(newPanel);
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
        vector<vector<int>> pan;
        for (auto p : panel) {
            bool fail = false;
            for (auto v : p) {
                if (find(ad.begin(), ad.end(), v) == ad.end()) {
                    fail = true;
                    break;
                }
            }
            if (!fail)
                pan.push_back(p);
        }
        vector<vector<int>> modifiedPan;
        for (auto p : pan) {
            vector<int> newPanel;
            for (auto v : p) {
                int index = (int)(find(ad.begin(), ad.end(), v) - ad.begin());
                newPanel.push_back(index);
            }
            modifiedPan.push_back(newPanel);
        }
        addAdjacents(adj, modifiedPan);
    }
//    End: Set disjoint mapped adjacent list
}

void PaneledIsomorphism::addAdjacents(vector<bool*> _adjacent, vector<vector<int>> _panel)
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

//    Edit panels since we mapped ids in order of degrees.
    vector<vector<int>> oPanel;
    for (auto p : _panel) {
        vector<int> newPanel;
        for (auto v : p) {
            int index = (int)(find(degMap.begin(), degMap.end(), v) - degMap.begin());
            newPanel.push_back(index);
        }
        oPanel.push_back(newPanel);
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
    vector<vector<int>> newPanel = _panel;
    vector<vector<int>> bestPanel = _panel;
    bestAdjacentRepresentation[0] = 10000;
    if (bestPanel.size() != 0)
        for (int i = 0; i < bestPanel[0].size(); ++i)
            bestPanel[0][i] = 21;
    sortPanels(bestPanel);
    vector<bool*> newAdjacent;
    for (int i = 0; i < _adjacent.size(); ++i) {
        bool* oneAdjacent = new bool[ADJACENT_MAX];
        for (int a = 0; a < ADJACENT_MAX; ++a)
            oneAdjacent[a] = false;
        newAdjacent.push_back(oneAdjacent);
    }
    vector<int> BEST;
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
        }
        vector<vector<int>> oPanel;
        for (auto p : _panel) {
            vector<int> nP;
            for (auto v : p) {
                int index = (int)(find(pairs[i].begin(), pairs[i].end(), v) - pairs[i].begin());
                nP.push_back(index);
            }
            oPanel.push_back(nP);
        }
        newPanel = oPanel;
        sort(newPanel.begin(), newPanel.end());
        setAdjacentRepresentation(newAdjacent, newAdjacentRepresentation);
        Results result = compareAdjacent(bestAdjacentRepresentation, repCount, newAdjacentRepresentation, repCount);
        sortPanels(newPanel);
        if (result == Results::BETTER) {
            for (int r = 0; r < repCount; ++r)
                bestAdjacentRepresentation[r] = newAdjacentRepresentation[r];
            bestPanel = newPanel;
        } else if (result == Results::EQUIVALENT && comparePanel(bestPanel, newPanel) == Results::BETTER) {
            bestPanel = newPanel;
            BEST = pairs[i];
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
        cerr << "detected three or more separated graphs in isomorphic!" << endl;
        exit(1);
    }

    return false;
}

vector<int> PaneledIsomorphism::getAdjacentIndexies(const vector<int>& _face, const vector<vector<int>>& _facesList) const
{
    vector<int> indexies;
    for (int i = 0; i < _facesList.size(); ++i) {
        if (
            (find(_facesList[i].begin(), _facesList[i].end(), _face[0]) != _facesList[i].end() &&
             find(_facesList[i].begin(), _facesList[i].end(), _face[1]) != _facesList[i].end()) ||
            (find(_facesList[i].begin(), _facesList[i].end(), _face[1]) != _facesList[i].end() &&
             find(_facesList[i].begin(), _facesList[i].end(), _face[2]) != _facesList[i].end()) ||
            (find(_facesList[i].begin(), _facesList[i].end(), _face[0]) != _facesList[i].end() &&
             find(_facesList[i].begin(), _facesList[i].end(), _face[2]) != _facesList[i].end())
            ) {
            indexies.push_back(i);
        }
    }
    return indexies;
}

vector<vector<int>> PaneledIsomorphism::getConcatFace(const vector<vector<int>>& _facesList)
{
    vector<pair<int, int>> edges;
    for (auto face : _facesList) {
        for (int i = 0; i < 3; ++i) {
            pair<int, int> newPair;
            int index1 = i % 3;
            int index2 = (i + 1) % 3;
            if (face[index1] < face[index2])
                newPair = make_pair(face[index1], face[index2]);
            else
                newPair = make_pair(face[index2], face[index1]);
            auto duplicateIterator = find(edges.begin(), edges.end(), newPair);
            if (duplicateIterator == edges.end())
                edges.push_back(newPair);
            else
                edges.erase(duplicateIterator);
        }
    }

    map<int, bool> mapping;
    for (auto ed : edges) {
        mapping[ed.first] = false;
        mapping[ed.second] = false;
    }
    vector<vector<int>> sequencesSet;
    while (find_if(mapping.begin(), mapping.end(), [](const pair<int, bool>& pair) { return !pair.second; }) != mapping.end()) {
        vector<int> sequence;
        int start = -1;
        for (auto m : mapping)
            if (!m.second)
                start = m.first;
        vector<pair<int, int>> temp2;
        for (auto ed : edges)
            if (ed.first == start || ed.second == start)
                temp2.push_back(ed);
        int end = (temp2.front().first == start) ? temp2.front().second : temp2.front().first;
        while (sequence.size() < edges.size()) {
            sequence.push_back(start);
            mapping[start] = true;
            vector<int> candidates;
            for (auto ed : edges) {
                if ((end == ed.first || end == ed.second) && (start != ed.first && start != ed.second)) {
                    if (end == ed.first)
                        candidates.push_back(ed.second);
                    else
                        candidates.push_back(ed.first);
                }
            }
            if (candidates.size() == 1) {
                start = end;
                end = candidates[0];
            } else {
                int setId = getFromCandiate(end, start, _facesList, candidates, edges);
                start = end;
                end = setId;
            }
        }
        int removeIndex = -1;
        for (int i = 2; i < sequence.size() - 1; ++i)
            if (sequence[i] == sequence[0] && sequence[i + 1] == sequence[1])
                removeIndex = i;
        if (removeIndex != -1)
            sequence.erase(sequence.begin() + removeIndex, sequence.end());
        sequencesSet.push_back(sequence);
    }

    return sequencesSet;
}

int PaneledIsomorphism::getFromCandiate(int _start, int _end, const vector<vector<int>>& _faces, const vector<int>& _candiate, const vector<pair<int, int>>& _remainEds)
{
    int prev = -1;
    vector<int> adjacentFace;
    int loopDetectCount = 0;
    while (true) {
        for (auto f : _faces) {
            if (find(f.begin(), f.end(), _start) != f.end() &&
                find(f.begin(), f.end(), _end) != f.end() &&
                find(f.begin(), f.end(), prev) == f.end())
                adjacentFace = f;
        }
        int other = -1;
        if ((adjacentFace[0] == _start && adjacentFace[1] == _end) || (adjacentFace[0] == _end && adjacentFace[1] == _start))
            other = adjacentFace[2];
        else if ((adjacentFace[1] == _start && adjacentFace[2] == _end) || (adjacentFace[1] == _end && adjacentFace[2] == _start))
            other = adjacentFace[0];
        else if ((adjacentFace[0] == _start && adjacentFace[2] == _end) || (adjacentFace[0] == _end && adjacentFace[2] == _start))
            other = adjacentFace[1];
        if (find(_candiate.begin(), _candiate.end(), other) != _candiate.end() && (find(_remainEds.begin(), _remainEds.end(), make_pair(other, _start)) != _remainEds.end() || find(_remainEds.begin(), _remainEds.end(), make_pair(_start, other)) != _remainEds.end()))
            return other;
        prev = _end;
        _end = other;
        loopDetectCount++;
        if (loopDetectCount > 1000) {
            cerr << "Loop detected!!" << endl;
            exit(1);
        }
    }
}

vector<vector<int>> PaneledIsomorphism::getConvertedFaces(vector<vector<int>> _facesList)
{
    queue<vector<int>> stock;
    vector<vector<vector<int>>> facesSet;
    while (_facesList.size() > 0) {
        vector<int> face = _facesList[0];
        _facesList.erase(_facesList.begin());
        facesSet.push_back(vector<vector<int>>());
        facesSet.back().push_back(face);
        stock.push(face);
        while (!stock.empty()) {
            face = stock.front();
            stock.pop();
            vector<int> indexies = getAdjacentIndexies(face, _facesList);
            for (int i = (int)indexies.size() - 1; i >= 0; --i) {
                stock.push(_facesList[indexies[i]]);
                facesSet.back().push_back(_facesList[indexies[i]]);
                _facesList.erase(_facesList.begin() + indexies[i]);
            }
        }
    }

    vector<vector<int>> newFaces;
    for (auto faces : facesSet) {
        auto results = getConcatFace(faces);
        copy(results.begin(), results.end(), back_inserter(newFaces));
    }

    return newFaces;
}

bool PaneledIsomorphism::isomorphicOne(const array<int, GraphProperty::MAX_EDGE> &_adj1, const vector<vector<int>> &_panel1, int _repCount1, const array<int, GraphProperty::MAX_EDGE> &_adj2, const vector<vector<int>> &_panel2, int _repCount2) const
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

Isomorphism::Results PaneledIsomorphism::comparePanel(const vector<vector<int>>& _p1, const vector<vector<int>>& _p2) const
{
    if (_p1.size() != _p2.size()) return Results::FAIL;
    for (int i = 0; i < _p1.size(); ++i) {
        if (_p1[i].size() < _p2[i].size()) return Results::FAIL;
        else if (_p1[i].size() > _p2[i].size()) return Results::BETTER;
        for (int j = 0; j < _p1[i].size(); ++j) {
            if (_p1[i][j] < _p2[i][j])
                return Results::FAIL;
            else if (_p1[i][j] > _p2[i][j])
                return Results::BETTER;
        }
    }

    return Results::EQUIVALENT;
}

void PaneledIsomorphism::sortPanels(vector<vector<int>>& _panels)
{
    for (int i = 0; i < _panels.size(); ++i)
        _panels[i] = sortedPanel(_panels[i]);
    sort(_panels.begin(), _panels.end(), [](const vector<int>& a, const vector<int>& b) -> bool {
        int firstA = a[0];
        int secondA = a[1];
        int firstB = b[0];
        int secondB = b[1];
        return firstA + secondA * 1000 < firstB + secondB * 1000;
    });
}

vector<int> PaneledIsomorphism::sortedPanel(vector<int>& _panel)
{
    int minIndex = (int)(min_element(_panel.begin(), _panel.end()) - _panel.begin());
    int direct = (_panel[(minIndex + 1) % _panel.size()] < _panel[(minIndex - 1 + _panel.size()) % _panel.size()]) ? 1 : -1;
    vector<int> sorted;
    sorted.push_back(_panel[minIndex]);
    for (int i = (minIndex + direct + (int)_panel.size()) % _panel.size(); i != minIndex; i = (i + direct + (int)_panel.size()) % _panel.size())
        sorted.push_back(_panel[i]);

    return sorted;
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
            for (auto v : onePanel)
                panelString += to_string(v) + ":";
            panelString.back() = ',';
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
        vector<vector<int>> onePanels;
        for (int o = 0; o < panelStrings.size(); ++o) {
            vector<int> ps;
            vector<string> vertices = Utility::split(panelStrings[o], ':');
            for (auto v : vertices)
                ps.push_back(atoi(v.c_str()));
            onePanels.push_back(ps);
        }

        bestAdjacentRepresentationList.push_back(oneRep);
        panelsList.push_back(onePanels);
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
