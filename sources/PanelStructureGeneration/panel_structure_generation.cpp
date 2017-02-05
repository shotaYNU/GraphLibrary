#include "panel_structure_generation.hpp"

PanelStrctureGeneration::PanelStrctureGeneration()
{

}

PanelStrctureGeneration::~PanelStrctureGeneration()
{

}

void PanelStrctureGeneration::generate(PaneledGraph* _graph)
{
    graph = _graph;
    embeddingSet = new EmbeddingSet();
    embeddingMatrix.clear();
    faces.clear();
    existPattern.clear();

    vector<long long> originFaces;
    vector<long long> newFaces;
    PaneledFace* face;
    EmbeddedEdge *ed, *end;

    _graph->initFace();
    for (int i = 0; i < _graph->getVerticesNum(); ++i) {
        ed = end = _graph->getVertex(i)->getFirstEdge();
        do {
            face = (PaneledFace*)ed->getNextFace();
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

    vector<vector<bool>> base;
    embeddingSet->setAllEmbeddings(_graph);
    for (int e = 0; e < embeddingSet->getInequivalentEmbeddingsNum(); ++e) {
        newFaces = originFaces;
        for (auto transposition : embeddingSet->getInequivalentEmbeddings(e))
            Utility::allExchangeLong(transposition.first, transposition.second, newFaces);
        vector<bool> oneSet;
        for (int i = 0; i < faces.size(); ++i)
            oneSet.push_back(find(newFaces.begin(), newFaces.end(), originFaces[i]) != newFaces.end());
        base.push_back(oneSet);
    }

    for (int i = 0; i < base.front().size(); ++i) {
        vector<bool> oneSet;
        for (int j = 0; j < base.size(); ++j)
            oneSet.push_back(base[j][i]);
        embeddingMatrix.push_back(oneSet);
    }

    vector<int> defaultSet;
    vector<int> rest;
    vector<bool> allTrue;
    for (int i = 0; i < embeddingMatrix.front().size(); ++i)
        allTrue.push_back(true);

    for (int i = 0; i < embeddingMatrix.size(); ++i) {
        if (getProductSet(embeddingMatrix[i], allTrue) == allTrue) defaultSet.push_back(i);
        else rest.push_back(i);
    }
    addIfNotExist(defaultSet, embeddingSet->getInequivalentEmbeddingsNum(), allTrue);
    settingRecursive(defaultSet, rest);
}

void PanelStrctureGeneration::settingRecursive(const vector<int>& _selectedIndexies, const vector<int>& _restIndexies)
{
    if (_restIndexies.size() == 0) return;
    for (auto index : _restIndexies) {
        vector<int> indexies = _selectedIndexies;
        indexies.push_back(index);

        tuple<vector<int>, vector<int>, int, vector<bool>> results = select(indexies);
        vector<int> selected = get<0>(results);
        vector<int> rest = get<1>(results);
        int embeddingNum = get<2>(results);
        if (embeddingNum == -1) return;
        vector<bool> emb = get<3>(results);

        addIfNotExist(selected, embeddingNum, emb);
        settingRecursive(selected, rest);
    }
    return;
}

void PanelStrctureGeneration::addIfNotExist(const vector<int>& _selectIndxies, int _embeddingNum, const vector<bool>& _emb)
{
    for (auto face : faces)
        face->resetPanel();
    for (auto index : _selectIndxies)
        faces[index]->setPanel();
    if (!isFullPanel(faces)) {
        PaneledGraphRepresentation graphRep(graph);
        graphRep.setBestRepresentation();
        bool save = false;
        bool disjointCase = false;
        PaneledIsomorphism iso(graph);
        if (graphRep.getTraversedAll()) {
            if (contains(graphRep) == -1) {
                graphRepresentations.push_back(graphRep);
                save = true;
            }
        } else {
            if (contains(graphRep) == -1) {
                graphRepresentations.push_back(graphRep);
                save = true;
                disjointCase = true;
            }
        }
        if (save) {
            picojson::array embStrings;
            for (int e = 0; e < _emb.size(); ++e) {
                if (!_emb[e]) continue;
                vector<int> permutation = Utility::convertToPermutation(embeddingSet->getInequivalentEmbeddings(e), graph->getVerticesNum());
                string embeddingString = "";
                for (auto transposition : embeddingSet->getInequivalentEmbeddings(e)) {
                    embeddingString += get<0>(EmbeddedVertex::idToChar(transposition.first, false));
                    embeddingString += get<0>(EmbeddedVertex::idToChar(transposition.second, false));
                    embeddingString += ",";
                }
                embeddingString.pop_back();
                embStrings.push_back(picojson::value(embeddingString));
            }
            vector<pair<string, picojson::value>> appendDatas{
                make_pair("embeddingNum", picojson::value((double)_embeddingNum)),
                make_pair("embeddings", picojson::value(embStrings))
            };
            if (disjointCase) {
                fileSaveDispatcher2.save(graph->toSaveGraph(appendDatas));
            } else {
                fileSaveDispatcher.save(graph->toSaveGraph(appendDatas));
            }
        }
    }
}

bool PanelStrctureGeneration::isFullPanel(const vector<PaneledFace*>& _faces)
{
    for (auto face : _faces)
        if (!face->getPanel())
            return false;

    return true;
}

int PanelStrctureGeneration::contains(const PaneledGraphRepresentation& _newGraphRepresentation)
{
    for (int i = 0; i < graphRepresentations.size(); ++i)
        if (graphRepresentations[i].compareRepresentation(_newGraphRepresentation) == Representation::Results::AUTOMORPHISM)
            return i;

    return -1;
}

int PanelStrctureGeneration::containsIsomorphism(const PaneledIsomorphism& _newIsomorphism)
{
    for (int i = 0; i < isomorphisms.size(); ++i)
        if (isomorphisms[i].isomorphic(_newIsomorphism))
            return i;

    return -1;
}

vector<bool> PanelStrctureGeneration::getProductSet(const vector<bool>& _set1, const vector<bool>& _set2) const
{
    vector<bool> result;
    for (int i = 0; i < _set1.size(); ++i)
        result.push_back(_set1[i] && _set2[i]);

    return result;
}

tuple<vector<int>, vector<int>, int, vector<bool>> PanelStrctureGeneration::select(const vector<int>& _selectIndexies)
{
    vector<int> facesList;
    vector<int> othersList;
    vector<bool> origin;

    for (int i = 0; i < embeddingMatrix.front().size(); ++i)
        origin.push_back(true);
    for (auto index : _selectIndexies)
        origin = getProductSet(origin, embeddingMatrix[index]);

    if (find(existPattern.begin(), existPattern.end(), _selectIndexies) == existPattern.end())
        existPattern.push_back(_selectIndexies);
    else
        return make_tuple(vector<int>(), vector<int>(), -1, vector<bool>());

    for (int i = 0; i < embeddingMatrix.size(); ++i) {
        vector<bool> product = getProductSet(origin, embeddingMatrix[i]);
        if (product == origin)
            facesList.push_back(i);
        else
            othersList.push_back(i);
    }

    int embeddingNum = 0;
    for (auto bl : origin)
        if (bl) embeddingNum++;

    return make_tuple(facesList, othersList, embeddingNum, origin);
}

vector<pair<PaneledGraphRepresentation, int>> PanelStrctureGeneration::getAllInformations(PaneledGraph& _graph)
{
    vector<PaneledFace*> faces;
    vector<long long> originFaces;
    vector<long long> newFaces;
    PaneledFace* face;
    EmbeddedEdge *ed, *end;
    vector<int> selectFaces;

    for (int i = 0; i < _graph.getVerticesNum(); ++i) {
        ed = end = _graph.getVertex(i)->getFirstEdge();
        do {
            face = (PaneledFace*)ed->getNextFace();
            if (find(faces.begin(), faces.end(), face) == faces.end())
                faces.push_back(face);
            ed = ed->getNext();
        } while(ed != end);
    }

    for (int f = 0; f < faces.size(); ++f) {
        if (((PaneledFace*)faces[f])->getPanel())
            selectFaces.push_back(f);
        originFaces.push_back(0ULL);
        for (auto vertex : faces[f]->getFace())
            originFaces.back() += (1ULL << vertex);
    }

    vector<pair<PaneledGraphRepresentation, int>> representations;
    for (int e = 0; e < _graph.embeddings.size(); ++e) {
        newFaces = originFaces;
        for (auto transposition : _graph.embeddings[e])
            Utility::allExchangeLong(transposition.first, transposition.second, newFaces);
        for (auto face : faces)
            face->resetPanel();
        for (auto index : selectFaces) {
            int select = (int)(find(newFaces.begin(), newFaces.end(), originFaces[index]) - newFaces.begin());
            faces[select]->setPanel();
        }
        PaneledGraphRepresentation rep(&_graph);
        rep.setBestRepresentation();
        int existIndex = -1;
        for (int t = 0; t < representations.size(); ++t)
            if (rep.compareRepresentation(representations[t].first) == Representation::Results::AUTOMORPHISM) {
                existIndex = t;
                break;
            }
        if (existIndex == -1) {
            representations.push_back(make_pair(rep, 1));
        } else
            representations[existIndex].second++;
    }

    //Reset panels
    for (auto face : faces)
        face->resetPanel();
    for (auto index : selectFaces)
        faces[index]->setPanel();

    return representations;
}

void PanelStrctureGeneration::omitDuplicates(const vector<PaneledGraph*>& _graphs, FileSaveDispatcher& _fileSaveDispatcher)
{
    vector<PaneledIsomorphism> isomorphisms;
    vector<vector<pair<PaneledGraphRepresentation, int>>> informations;
    vector<int> indexies;
    int count = 0;
    for (auto graph : _graphs) {
        PaneledIsomorphism isomorphism(graph);
        vector<pair<string, string>> datas;
        for (int c = 0; c < graph->isomorphismsForIsomorphism.size(); ++c)
            datas.push_back(make_pair(graph->isomorphismsForIsomorphism[c], graph->panelsForIsomorphism[c]));
        isomorphism.setPaneledIsomorphismDatas(datas);
        isomorphisms.push_back(isomorphism);
        informations.push_back(PanelStrctureGeneration::getAllInformations(*graph));
        indexies.push_back(count);
        cout << ++count << endl;
    }

    vector<PaneledIsomorphism> existIsomorphisms;
    vector<vector<pair<PaneledGraphRepresentation, int>>> existInformations;
    vector<int> existIndexies;
    for (int i = 0; i < isomorphisms.size(); ++i) {
        int existIndex = -1;
        for (int t = 0; t < existIsomorphisms.size(); ++t)
            if (isomorphisms[i].isomorphic(existIsomorphisms[t])) {
                existIndex = t;
                break;
            }
        if (existIndex == -1) {
            existIsomorphisms.push_back(isomorphisms[i]);
            existInformations.push_back(informations[i]);
            existIndexies.push_back(i);
        } else {
            for (auto info : informations[i]) {
                int existIndex2 = -1;
                for (int t = 0; t < existInformations[existIndex].size(); ++t)
                    if (info.first.compareRepresentation(existInformations[existIndex][t].first) == Representation::Results::AUTOMORPHISM) {
                        existIndex2 = t;
                        break;
                    }
                if (existIndex2 == -1) {
                    existInformations[existIndex].push_back(info);
                    cout << "---  Congruent case  ---: " << i << " to " << existIndex << " of " << existIndexies[existIndex] << endl;
                }
            }
            cout << "---  case  ---: " << i << " to " << existIndex << " of " << existIndexies[existIndex] << endl;
        }
    }

    for (int i = 0; i < existIsomorphisms.size(); ++i) {
        int embeddingNum = 0;
        for (auto info : existInformations[i])
            embeddingNum += info.second;
        vector<pair<string, picojson::value>> appendDatas{
            make_pair("embeddingNum", picojson::value((double)embeddingNum))
        };
        _fileSaveDispatcher.save(_graphs[existIndexies[i]]->toSaveGraph(appendDatas));
    }

    cout << "Remain: " << existIsomorphisms.size() << endl;
}
