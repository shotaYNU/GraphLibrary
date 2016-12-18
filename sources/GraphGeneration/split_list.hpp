#ifndef SplitList_hpp
#define SplitList_hpp

#include "mutable_graph.hpp"
#include "../GraphOperation/automorphism.hpp"
#include <map>

class SplitList {
public:
    //A constructor and a destructor.
    SplitList(MutableGraph* _graph);
    ~SplitList();

    //Getter methods and setter methods.
    int getSplitDeg(int count) const { return splitDegList[count]; }
    int getSplitCount() const { return splitCount; }
    void setSplitListAll(Automorphism* aut);
    EmbeddedEdge* getSplitEdge(int count) const { return splitEdgeList[count]; }

protected:

private:
    MutableGraph* graph;
    Automorphism* aut;
    EmbeddedEdge* splitEdgeList[GraphProperty::MAX_EDGE];
    int splitDegList[GraphProperty::MAX_EDGE];
    int splitCount;
    int minDegNum;
    int minDeg;
    int total;
    int mirrorStart;

    void markAllMappingEdge(bool _clockwise, EmbeddedEdge* _ed, int _start, int _end, int _deg);
    void setSplitListDeg3();
    void setSplitListDeg4();
    void setSplitListDegn(int _n);
    void setSplitListDegnPlus();
    void init(Automorphism* _aut);

};

#endif
