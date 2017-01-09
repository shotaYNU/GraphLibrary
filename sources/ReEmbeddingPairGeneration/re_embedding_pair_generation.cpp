#include "re_embedding_pair_generation.hpp"

ReEmbeddingPairGeneration::ReEmbeddingPairGeneration()
{

}

ReEmbeddingPairGeneration::~ReEmbeddingPairGeneration()
{

}

void ReEmbeddingPairGeneration::generate(PaneledGraph* _graph)
{
    vector<PaneledFace*> faces;
    vector<long long> originFaces;
    vector<long long> newFaces;
    EmbeddingSet embeddingSet;
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

    embeddingSet.setAllEmbeddings(_graph);
    for (int e = 0; e < embeddingSet.getEmbeddingsNum(); ++e) {
        newFaces = originFaces;
        for (auto transposition : embeddingSet.getEmbeddings(e))
            Utility::allExchange(transposition.first, transposition.second, newFaces);
    for (int i = 0; i < faces.size(); ++i) {
            if (find(newFaces.begin(), newFaces.end(), originFaces[i]) == newFaces.end())
                faces[i]->resetPanel();
            else
                faces[i]->setPanel();
        }
        if (isFullPanel(faces) || !_graph->isPIT()) continue;
        PaneledGraphRepresentation graphRep(_graph);
        graphRep.setBestRepresentation();
        if (!contains(graphRep)) {
            graphRepresentations.push_back(graphRep);
//            ((PaneledGraph*)_graph)->saveGraph("");
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
