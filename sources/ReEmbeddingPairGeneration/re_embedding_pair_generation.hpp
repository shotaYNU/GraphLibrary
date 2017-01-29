#ifndef ReEmbeddingPairGeneration_hpp
#define ReEmbeddingPairGeneration_hpp

#include "paneled_graph.hpp"
#include "paneled_graph_representation.hpp"
#include "embedding_set.hpp"
#include "file_save_dispatcher.hpp"
#include "paneled_isomorphism.hpp"
#include "progress.hpp"

class ReEmbeddingPairGeneration {
public:
    ReEmbeddingPairGeneration();
    ~ReEmbeddingPairGeneration();

    void generate(vector<PaneledGraph*>& _graphs);
    void generate(PaneledGraph* _graph1, PaneledGraph* _graph2);
    int getPairsNum() { return (int)graphRepresentations.size(); }
    void setSaveSetting(string _saveDirectoryPath, string _commonName) { fileSaveDispatcher.setSaveSetting(_saveDirectoryPath, _commonName); }

protected:

private:
    vector<PaneledGraphRepresentation> graphRepresentations;
    vector<PaneledIsomorphism> isomorphisms;
    FileSaveDispatcher fileSaveDispatcher;

    bool isFullPanel(const vector<PaneledFace*>& _faces);
    bool contains(const PaneledGraphRepresentation& _newGraphRepresentation);
    bool containsIsomorphism(const PaneledIsomorphism& _newIsomorphism);

};

#endif
