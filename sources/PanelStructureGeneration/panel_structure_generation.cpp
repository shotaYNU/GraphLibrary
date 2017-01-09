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
            Utility::allExchange(transposition.first, transposition.second, newFaces);
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
    addIfNotExist(defaultSet, embeddingSet->getInequivalentEmbeddingsNum());
    settingRecursive(defaultSet, rest);
}

void PanelStrctureGeneration::settingRecursive(const vector<int>& _selectedIndexies, const vector<int>& _restIndexies)
{
    if (_restIndexies.size() == 0) return;
    for (auto index : _restIndexies) {
        vector<int> indexies = _selectedIndexies;
        indexies.push_back(index);

        tuple<vector<int>, vector<int>, int> results = select(indexies);
        vector<int> selected = get<0>(results);
        vector<int> rest = get<1>(results);
        int embeddingNum = get<2>(results);

        addIfNotExist(selected, embeddingNum);
        settingRecursive(selected, rest);
    }
    return;
}

void PanelStrctureGeneration::addIfNotExist(const vector<int>& _selectIndxies, int _embeddingNum)
{
    for (auto face : faces)
        face->resetPanel();
    for (auto index : _selectIndxies)
        faces[index]->setPanel();
    if (!isFullPanel(faces) && graph->isPIT()) {
        PaneledGraphRepresentation graphRep(graph);
        graphRep.setBestRepresentation();
        if (!contains(graphRep)) {
            graphRepresentations.push_back(graphRep);
//            graph->saveGraph("");
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

bool PanelStrctureGeneration::contains(const PaneledGraphRepresentation& _newGraphRepresentation)
{
    for (auto graphRep : graphRepresentations)
        if (graphRep.compareRepresentation(_newGraphRepresentation) == Representation::Results::AUTOMORPHISM)
            return true;

    return false;
}

vector<bool> PanelStrctureGeneration::getProductSet(const vector<bool>& _set1, const vector<bool>& _set2) const
{
    vector<bool> result;
    for (int i = 0; i < _set1.size(); ++i)
        result.push_back(_set1[i] && _set2[i]);

    return result;
}

tuple<vector<int>, vector<int>, int> PanelStrctureGeneration::select(const vector<int>& _selectIndexies) const
{
    vector<int> facesList;
    vector<int> othersList;
    vector<bool> origin;

    for (int i = 0; i < embeddingMatrix.front().size(); ++i)
        origin.push_back(true);
    for (auto index : _selectIndexies)
        origin = getProductSet(origin, embeddingMatrix[index]);

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

    return make_tuple(facesList, othersList, embeddingNum);
}
