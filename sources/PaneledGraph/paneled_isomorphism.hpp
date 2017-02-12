#ifndef paneled_isomorphism_hpp
#define paneled_isomorphism_hpp

#include "../GraphOperation/isomorphism.hpp"
#include "paneled_graph.hpp"
#include <queue>
#include <array>
#include "file_save_dispatcher.hpp"

class PaneledIsomorphism: public Isomorphism {
public:
    //Constructors and a destructor.
    PaneledIsomorphism(PaneledGraph* _graph);
    ~PaneledIsomorphism();

    //Getter methods and setter methods
    void setMappedAdjacents();
    bool isomorphic(const Isomorphism& isomorphism) const;
    void addAdjacents(vector<bool*> _adjacent, vector<vector<int>> _panel);
    vector<pair<string, string>> toStringDatas();
    void setPaneledIsomorphismDatas(const vector<pair<string, string>>& _datas);
    static void saveIsomorphisms(const vector<PaneledGraph*>& _graphs, FileSaveDispatcher& _fileSaveDispatcher);

protected:

private:
    vector<vector<vector<int>>> panelsList;
    vector<array<int, GraphProperty::MAX_EDGE>> bestAdjacentRepresentationList;
    vector<int> repCounts;
    bool isomorphicOne(const array<int, GraphProperty::MAX_EDGE>& _adj1, const vector<vector<int>>& _panel1, int _repCount1, const array<int, GraphProperty::MAX_EDGE>& _adj2, const vector<vector<int>>& _panel2, int _repCount2) const;

    Results compareAdjacent(const array<int, GraphProperty::MAX_EDGE>& _adj1, int _repCount1, const array<int, GraphProperty::MAX_EDGE>& _adj2, int _repCount2) const;
    Results comparePanel(const vector<vector<int>>& _p1, const vector<vector<int>>& _p2) const;

    vector<int> getAdjacentIndexies(const vector<int>& _face, const vector<vector<int>>& _facesList) const;
    vector<vector<int>> getConcatFace(const vector<vector<int>>& _facesList);
    int getFromCandiate(int _start, int _end, const vector<vector<int>>& _faces, const vector<int>& _candiate, const vector<pair<int, int>>& _remainEds);
    vector<vector<int>> getConvertedFaces(vector<vector<int>> _facesList);
    void sortPanels(vector<vector<int>>& _panels);
    vector<int> sortedPanel(vector<int>& _panel);

};

#endif
