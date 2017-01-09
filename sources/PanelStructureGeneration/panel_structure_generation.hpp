#ifndef PanelStrctureGenerationGeneration_hpp
#define PanelStrctureGenerationGeneration_hpp

#include "paneled_graph.hpp"
#include "paneled_graph_representation.hpp"
#include "embedding_set.hpp"

class PanelStrctureGeneration {
public:
    PanelStrctureGeneration();
    ~PanelStrctureGeneration();

    void generate(PaneledGraph* _graph);
    int getPanelStrcturesNum() { return (int)graphRepresentations.size(); }

protected:

private:
    PaneledGraph* graph;
    vector<PaneledGraphRepresentation> graphRepresentations;
    vector<vector<bool>> embeddingMatrix;
    EmbeddingSet* embeddingSet;
    vector<PaneledFace*> faces;

    void addIfNotExist(const vector<int>& _selectIndxies, int _embeddingNum);
    bool isFullPanel(const vector<PaneledFace*>& _faces);
    int contains(const PaneledGraphRepresentation& _newGraphRepresentation);
    vector<bool> getProductSet(const vector<bool>& _set1, const vector<bool>& _set2) const;
    tuple<vector<int>, vector<int>, int> select(const vector<int>& _selectIndexies) const;
    void settingRecursive(const vector<int>& _selectedIndexies, const vector<int>& _restIndexies);

};

#endif
