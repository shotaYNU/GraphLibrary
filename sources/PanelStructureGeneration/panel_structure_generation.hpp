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
    void setSaveSetting(string _saveDirectoryPath, string _commonName) { fileSaveDispatcher.setSaveSetting(_saveDirectoryPath, _commonName); fileSaveDispatcher2.setSaveSetting(_saveDirectoryPath + "Disjoint/", _commonName); }

    static void omitDuplicates(const vector<PaneledGraph*>& _graphs, FileSaveDispatcher& _fileSaveDispatcher);
    static vector<pair<PaneledGraphRepresentation, int>> getAllInformations(PaneledGraph& _graph);
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
    FileSaveDispatcher fileSaveDispatcher2;
    vector<vector<int>> existPattern;

    void addIfNotExist(const vector<int>& _selectIndxies, int _embeddingNum, const vector<bool>& _emb);
    bool isFullPanel(const vector<PaneledFace*>& _faces);
    int contains(const PaneledGraphRepresentation& _newGraphRepresentation);
    int containsIsomorphism(const PaneledIsomorphism& _newIsomorphism);
    vector<bool> getProductSet(const vector<bool>& _set1, const vector<bool>& _set2) const;
    tuple<vector<int>, vector<int>, int, vector<bool>> select(const vector<int>& _selectIndexies);
    void settingRecursive(const vector<int>& _selectedIndexies, const vector<int>& _restIndexies);

};

#endif
