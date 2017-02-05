#ifndef PanelStrctureGenerationGeneration_hpp
#define PanelStrctureGenerationGeneration_hpp

#include "paneled_graph.hpp"
#include "paneled_graph_representation.hpp"
#include "embedding_set.hpp"
#include "paneled_isomorphism.hpp"
#include "file_save_dispatcher.hpp"

class PanelStrctureGeneration {
public:
    PanelStrctureGeneration();
    ~PanelStrctureGeneration();

    void generate(PaneledGraph* _graph);
    int getPanelStrcturesNum() { return (int)graphRepresentations.size(); }
    void setSaveSetting(string _saveDirectoryPath, string _commonName) { fileSaveDispatcher.setSaveSetting(_saveDirectoryPath, _commonName); }

protected:

private:
    PaneledGraph* graph;
    vector<PaneledGraphRepresentation> graphRepresentations;
    vector<PaneledIsomorphism> isomorphisms;
    vector<int> embeddings;
    vector<vector<bool>> embeddingMatrix;
    EmbeddingSet* embeddingSet;
    vector<PaneledFace*> faces;
    FileSaveDispatcher fileSaveDispatcher;
    vector<vector<int>> existPattern;

    void addIfNotExist(const vector<int>& _selectIndxies, int _embeddingNum);
    bool isFullPanel(const vector<PaneledFace*>& _faces);
    int contains(const PaneledGraphRepresentation& _newGraphRepresentation);
    int containsIsomorphism(const PaneledIsomorphism& _newIsomorphism);
    vector<bool> getProductSet(const vector<bool>& _set1, const vector<bool>& _set2) const;
    tuple<vector<int>, vector<int>, int> select(const vector<int>& _selectIndexies);
    void settingRecursive(const vector<int>& _selectedIndexies, const vector<int>& _restIndexies);

};

#endif
