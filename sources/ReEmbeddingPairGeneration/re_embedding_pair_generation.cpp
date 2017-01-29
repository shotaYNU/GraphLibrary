#include "re_embedding_pair_generation.hpp"

ReEmbeddingPairGeneration::ReEmbeddingPairGeneration()
{

}

ReEmbeddingPairGeneration::~ReEmbeddingPairGeneration()
{

}

void ReEmbeddingPairGeneration::generate(vector<PaneledGraph*>& _graphs)
{
    vector<Isomorphism> isos;
    for (int i = 0; i < _graphs.size(); ++i) {
        Isomorphism iso(_graphs[i]);
        iso.setMappedAdjacents();
        isos.push_back(iso);
    }
    vector<pair<PaneledGraph*, PaneledGraph*>> graphPairs;
    for (int i = 0; i < isos.size(); ++i)
        for (int j = i; j < isos.size(); ++j)
            if (isos[i].isomorphic(isos[j]))
                graphPairs.push_back(make_pair(_graphs[i], _graphs[j]));

    Progress progress;
    float count = 0;
    progress.startProgress();
    for (auto graphPair : graphPairs) {
        generate(graphPair.first, graphPair.second);
        progress.flushProgress(++count / float(graphPairs.size()));
    }
    progress.endProgress();
}

void ReEmbeddingPairGeneration::generate(PaneledGraph* _graph1, PaneledGraph* _graph2)
{
    vector<PaneledFace*> faces;
    vector<PaneledFace*> faces2;
    vector<long long> originFaces;
    vector<long long> compareFaces;
    vector<long long> newFaces;
    EmbeddingSet embeddingSet;
    PaneledFace* face;
    EmbeddedEdge *ed, *end;

    _graph1->initFace();
    for (int i = 0; i < _graph1->getVerticesNum(); ++i) {
        ed = end = _graph1->getVertex(i)->getFirstEdge();
        do {
            face = (PaneledFace*)ed->getNextFace();
            if (find(faces.begin(), faces.end(), face) == faces.end())
                faces.push_back(face);
            ed = ed->getNext();
        } while(ed != end);
    }
    for (auto face : faces) {
        originFaces.push_back(0ULL);
        for (auto vertex : face->getFace())
            originFaces.back() += (1ULL << vertex);
    }
    
    _graph2->initFace();
    for (int i = 0; i < _graph2->getVerticesNum(); ++i) {
        ed = end = _graph2->getVertex(i)->getFirstEdge();
        do {
            face = (PaneledFace*)ed->getNextFace();
            if (find(faces2.begin(), faces2.end(), face) == faces2.end())
                faces2.push_back(face);
            ed = ed->getNext();
        } while(ed != end);
    }
    for (auto face : faces2) {
        compareFaces.push_back(0ULL);
        for (auto vertex : face->getFace())
            compareFaces.back() += (1ULL << vertex);
    }

    embeddingSet.setAllEmbeddings(_graph2);
    for (int e = 0; e < embeddingSet.getEmbeddingsNum(); ++e) {
        newFaces = compareFaces;
        for (auto transposition : embeddingSet.getEmbeddings(e))
            Utility::allExchangeLong(transposition.first, transposition.second, newFaces);
        for (int i = 0; i < faces.size(); ++i) {
            if (find(newFaces.begin(), newFaces.end(), originFaces[i]) == newFaces.end())
                faces[i]->resetPanel();
            else
                faces[i]->setPanel();
        }
        if (isFullPanel(faces) || !_graph1->isPIT()) continue;
        PaneledGraphRepresentation graphRep(_graph1);
        graphRep.setBestRepresentation();
        bool save = false;
        PaneledIsomorphism iso(_graph1);
        if (graphRep.getTraversedAll()) {
            if (!contains(graphRep)) {
                iso.setMappedAdjacents();
                if (!containsIsomorphism(iso)) {
                    graphRepresentations.push_back(graphRep);
                    isomorphisms.push_back(iso);
                    save = true;
                }
            }
        } else {
            iso.setMappedAdjacents();
            if (!containsIsomorphism(iso)) {
                isomorphisms.push_back(iso);
                save = true;
            }
        }
        if (save) {
            vector<int> permutation = Utility::convertToPermutation(embeddingSet.getEmbeddings(e), _graph1->getVerticesNum());
            string embeddingString = "";
            for (auto p :  permutation)
                embeddingString += get<0>(EmbeddedVertex::idToChar(p, false));
            vector<pair<string, picojson::value>> appendDatas{
                make_pair("re-embedding", picojson::value(embeddingString)),
                make_pair("pair", picojson::value(_graph2->getName()))
            };
            fileSaveDispatcher.save(_graph1->toSaveGraph(appendDatas));
        }
    }
}

bool ReEmbeddingPairGeneration::isFullPanel(const vector<PaneledFace*>& _faces)
{
    for (auto face : _faces)
        if (!face->getPanel())
            return false;

    return true;
}

bool ReEmbeddingPairGeneration::contains(const PaneledGraphRepresentation& _newGraphRepresentation)
{
    for (auto graphRep : graphRepresentations)
        if (graphRep.compareRepresentation(_newGraphRepresentation) == Representation::Results::AUTOMORPHISM)
            return true;

    return false;
}

bool ReEmbeddingPairGeneration::containsIsomorphism(const PaneledIsomorphism &_newIsomorphism)
{
    for (auto iso : isomorphisms)
        if (iso.isomorphic(_newIsomorphism))
            return true;

    return false;
}
