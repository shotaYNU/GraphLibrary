#ifndef paneled_isomorphism_hpp
#define paneled_isomorphism_hpp

#include "../GraphOperation/isomorphism.hpp"
#include "paneled_graph.hpp"
#include <queue>
#include <array>

class PaneledIsomorphism: public Isomorphism {
public:
    //Constructors and a destructor.
    PaneledIsomorphism(PaneledGraph* _graph);
    ~PaneledIsomorphism();

    //Getter methods and setter methods
    void setMappedAdjacents();
    bool isomorphic(const Isomorphism& isomorphism) const;
    void addAdjacents(vector<bool*> _adjacent, vector<long long> _panel);

protected:

private:
    vector<vector<long long>> panelsList;
    vector<array<int, GraphProperty::MAX_EDGE>> bestAdjacentRepresentationList;
    vector<int> repCounts;
    bool isomorphicOne(const array<int, GraphProperty::MAX_EDGE>& _adj1, const vector<long long>& _panel1, int _repCount1, const array<int, GraphProperty::MAX_EDGE>& _adj2, const vector<long long>& _panel2, int _repCount2) const;

    Results compareAdjacent(const array<int, GraphProperty::MAX_EDGE>& _adj1, int _repCount1, const array<int, GraphProperty::MAX_EDGE>& _adj2, int _repCount2) const;
    Results comparePanel(const vector<long long>& _p1, const vector<long long>& _p2) const;

};

#endif
