#ifndef paneled_isomorphism_hpp
#define paneled_isomorphism_hpp

#include "../GraphOperation/isomorphism.hpp"
#include "paneled_graph.hpp"

class PaneledIsomorphism: public Isomorphism {
public:
    //Constructors and a destructor.
    PaneledIsomorphism(PaneledGraph* _graph);
    ~PaneledIsomorphism();

    //Getter methods and setter methods
    void setMappedAdjacents();
    bool isomorphic(const Isomorphism& isomorphism) const;

protected:

private:
    vector<vector<long long>> panels;

    bool equalPanel(const vector<long long>& _p1, const vector<long long>& _p2) const;

};

#endif
