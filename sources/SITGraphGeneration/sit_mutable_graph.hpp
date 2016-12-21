#ifndef SITMutableGraph_hpp
#define SITMutableGraph_hpp

#include "../GraphGeneration/mutable_graph.hpp"

class SITMutableGraph : public MutableGraph {
public:
    SITMutableGraph();
    ~SITMutableGraph();

    //Methods of skew.
    bool isSIT();

protected:

private:
    bool isSkew(int _id);
    bool isLinkSequence(const vector<int>& _origin_sequence, vector<int> _new_sequence);
    bool isIsomorphicSequence(const vector<int>& _new_sequence) const;

};

#endif
