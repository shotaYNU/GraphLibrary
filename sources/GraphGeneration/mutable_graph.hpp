#ifndef MutableGraph_hpp
#define MutableGraph_hpp

#include "../EmbeddedGraph/embedded_graph.hpp"

class MutableGraph : public EmbeddedGraph {
public:
    static const int EDGE_MAX_SIZE = GraphProperty::MAX_EDGE + (16 + 128) * GraphProperty::MAX_VERTEX;

    //A constructor and a destructor.
    MutableGraph();
    ~MutableGraph();

    //Operation of split and contraction.
    void splitDegn(EmbeddedEdge* _ed, int _n, EmbeddedEdge** _savelist);
    void contractDegn(EmbeddedEdge* _ed, int _n, EmbeddedEdge** _savelist);

    tuple<int, int> getContractibleMinDegree();
    void unMarkAllEdge();
    int makeColors(EmbeddedEdge* _ed);

protected:

private:
    EmbeddedEdge* edges[2 * EDGE_MAX_SIZE];
    EmbeddedEdge** addEdgeAddress;

    EmbeddedEdge** makeVertexDeg3();
    EmbeddedEdge** makeVertexDeg4();
    EmbeddedVertex* makeVertexDegn(int _n);
    EmbeddedEdge** getEdgeAddressDegn(int _vtxNum);

};

#endif
