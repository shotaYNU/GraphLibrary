#include "paneled_isomorphism.hpp"

PaneledIsomorphism::PaneledIsomorphism(PaneledGraph* _graph) : Isomorphism(_graph)
{
}

PaneledIsomorphism::~PaneledIsomorphism()
{
}

void PaneledIsomorphism::setMappedAdjacents()
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
        int deg = 0;
        do {
            if (((PaneledFace*)ed->getNextFace())->getPanel() && ((PaneledFace*)ed->getPrevFace())->getPanel()) {
                ed = ed->getNext();
                continue;
            }
            adjacent.back()[ed->getEnd()] = true;
            deg++;
            ed = ed->getNext();
        } while(ed != end);
        degrees.push_back(make_pair(deg, degrees.size()));
    }
    vector<int> removeList;
    for (int i = 0; i < adjacent.size(); ++i) {
        if (adjacent[i] == 0ULL)
            removeList.push_back(i);
    }
    reverse(removeList.begin(), removeList.end());
    for (auto index : removeList)
        adjacent.erase(adjacent.begin() + index);

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
//    change-for-panel-1
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
            for (int c = 0; c < 20; c++) {
                if ((set & (1ULL << c)) != 0)
                    count++;
            }
            if (count >= 3) {
                removeList.push_back(j);
            }else if (count >= 2) {
                removeList.push_back(i);
                removeList.push_back(j);
            }
        }
    }
    sort(removeList.begin(), removeList.end());
    vector<int> lists;
    prev = -1;
    for (int i = 0; i < removeList.size(); ++i) {
        if (prev != removeList[i]) {
            lists.push_back(removeList[i]);
            prev = removeList[i];
        }
    }
    reverse(lists.begin(), lists.end());
    for (auto index : lists)
        panel.erase(panel.begin() + index);

//    end-1

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

    vector<long long> oPanel;
    for (auto p : panel) {
        oPanel.push_back(0ULL);
        for (int i = 0; i < 20; ++i)
            if ((p & (1ULL << i)) != 0) {
                int index = (int)(find(degMap.begin(), degMap.end(), i) - degMap.begin());
                oPanel.back() += (1ULL << index);
            }
    }
    panel = oPanel;

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

        vector<long long> newPanel;
        for (auto pa : panel) {
            newPanel.push_back(0ULL);
            for (int k = 0; k < pairs[i].size(); ++k)
                if ((pa & (1ULL << k)) != 0) {
                    int index = pairs[i][k];
                    newPanel.back() += (1ULL << index);
                }
        }
        sort(newPanel.begin(), newPanel.end());
        panels.push_back(newPanel);
    }
}

bool PaneledIsomorphism::isomorphic(const Isomorphism& isomorphism) const
{
    if (mappedAdjacents.size() != isomorphism.getMappedAdjacents().size()) return false;

    for (int i = 0; i < mappedAdjacents.size(); ++i)
        if (equal(mappedAdjacents[i], isomorphism.getAdjacent()))
            if (equalPanel(panels[i], ((PaneledIsomorphism&)isomorphism).panels[0]))
                return true;

    return false;
}

bool PaneledIsomorphism::equalPanel(const vector<long long>& _p1, const vector<long long>& _p2) const
{
    if (_p1.size() != _p2.size()) return false;

    for (int i = 0; i < _p1.size(); ++i)
        if (_p1[i] != _p2[i])
            return false;

    return true;
}

