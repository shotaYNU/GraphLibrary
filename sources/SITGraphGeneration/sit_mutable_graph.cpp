#include "sit_mutable_graph.hpp"

SITMutableGraph::SITMutableGraph() : MutableGraph()
{
    
}

SITMutableGraph::~SITMutableGraph()
{
    
}

bool SITMutableGraph::isSIT()
{
    initContractible();
    for (int i = 0; i < verticesNum; ++i)
        if (vertices[i]->getIsContractible() && !isSkew(i))
            return false;

    return true;
}

bool SITMutableGraph::isSkew(int _id)
{
    EmbeddedEdge *ed, *end;
    vector<int> origin_sequence;
    vector<int> new_sequence;

    ed = end = vertices[_id]->getFirstEdge();
    do {
        origin_sequence.push_back(ed->getEnd());
        new_sequence.push_back(ed->getEnd());
        ed = ed->getNext();
    } while(ed != end);

    do {
        if (isIsomorphicSequence(new_sequence) && !isLinkSequence(origin_sequence, new_sequence))
            return true;
        next_permutation(new_sequence.begin(), new_sequence.end());
    } while (origin_sequence != new_sequence);

    return false;
}

bool SITMutableGraph::isIsomorphicSequence(const vector<int>& _new_sequence) const
{
    for (int i = 0; i < _new_sequence.size(); ++i) {
        int id1 = _new_sequence[i];
        int id2 = _new_sequence[(i + 1) % _new_sequence.size()];
        if (!vertices[id1]->getAdjacent(id2))
            return false;
    }

    return true;
}

bool SITMutableGraph::isLinkSequence(const vector<int>& _origin_sequence, vector<int> _new_sequence)
{
    int start = (int)(find(_new_sequence.begin(), _new_sequence.end(), _origin_sequence.front()) - _new_sequence.begin());

    rotate(_new_sequence.begin(), _new_sequence.begin() + start, _new_sequence.end());
    if (_origin_sequence == _new_sequence) return true;

    reverse(_new_sequence.begin() + 1, _new_sequence.end());
    if (_origin_sequence == _new_sequence) return true;

    return false;
}
