#ifndef EmbeddedVertex_hpp
#define EmbeddedVertex_hpp

#include "graph_property.hpp"
#include "embedded_edge.hpp"

class EmbeddedVertex {
public:
    //Constructors and a destructor.
    EmbeddedVertex();
    EmbeddedVertex(int _id);
    ~EmbeddedVertex();

    //Getter methods and setter methods.
    void setId(int _id);
    int getId() const { return id; }
    void setAdjacent(int _vertex) { adjacent[_vertex] = true; }
    void resetAdjacent(int _vertex) { adjacent[_vertex] = false; }
    void setDegree(int _deg) { degree = _deg; }
    int getDegree() const { return degree; }
    void setFirstEdge(EmbeddedEdge* _ed) { firstEdge = _ed; }
    EmbeddedEdge* getFirstEdge() const { return firstEdge; }
    void setIsContractible(bool _bl) { isContractible = _bl; }
    bool getIsContractible() const { return isContractible; }
    int getCommonCount(const EmbeddedVertex* _vtx) const;
    int getContractibleEdgeNum() const { return contractibleEdgesNum; }
    static int getCommonCount(const EmbeddedVertex* _vtx1, const EmbeddedVertex* _vtx2);

    //Initialize methods.
    void initContractibleEdgesNum() { contractibleEdgesNum = 0; }
    void initAdjacent();

    //Operation of contractible edges num.
    void increaseContractibleEdgesNum() { ++contractibleEdgesNum; }
    void decreaseContractibleEdgeNum() { --contractibleEdgesNum; }

    //Operation of degree of vertex.
    void increaseDegree(int _diff) { degree += _diff; }
    void decreaseDegree() { degree--; }

    //Methods to convert a data of vertex.
    static tuple<char, bool> idToChar(int _id, bool _flip);
    static tuple<int, bool> charToId(int _ch);

protected:
    int id;
    int degree;
    EmbeddedEdge* firstEdge;
    int adjacent[GraphProperty::MAX_VERTEX];
    bool isContractible;
    int contractibleEdgesNum;

private:

};

#endif
