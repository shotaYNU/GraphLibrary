#ifndef PaneledGraph_hpp
#define PaneledGraph_hpp

#include "../EmbeddedGraph/embedded_graph.hpp"
#include "paneled_face.hpp"
#include "embedding_set.hpp"

class PaneledGraph : public EmbeddedGraph {
public:
    //A constructor and a destructor.
    PaneledGraph();
    ~PaneledGraph();

    //Operation of panel.
    bool isPIT();
    bool isFlat(int _id);
    void saveGraph(string _filepath);
    void saveGraph(string _filepath, vector<pair<string, picojson::value>> _withData);
    void openGraph(string _filepath);
    string toSaveGraph();
    string toSaveGraph(vector<pair<string, picojson::value>> _withData);
    vector<vector<pair<int, int>>> embeddings;
    vector<string> panelsForIsomorphism;
    vector<string> isomorphismsForIsomorphism;


protected:
    EmbeddingSet embeddingSet;

private:
    void setPanelsWithAllEmbeddings();
    string toFacesString();
    void setPanel(string _panelString);

};

#endif
