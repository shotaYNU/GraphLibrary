#ifndef EmbeddedGraph_hpp
#define EmbeddedGraph_hpp

#include "graph_property.hpp"
#include "embedded_vertex.hpp"
#include "embedded_edge.hpp"

class EmbeddedGraph {
public:
    //A constructor and a destructor.
    EmbeddedGraph();
    ~EmbeddedGraph();

    //Getter methods and setter methods.
    int getVerticesNum() const { return verticesNum; }
    int getEdgesNum() const { return edgesNum; }
    void setOrientable(bool _bl) { orientable = _bl; }
    bool getOrientable() const { return orientable; }
    void setGenus(int _genus) { genus = _genus; }
    int getGenus() const { return genus; }
    int getVertexDegree(int _id) const { return vertices[_id]->getDegree(); }
    EmbeddedVertex* getVertex(int _id) const { return vertices[_id]; }
    bool getIrreducible() const { return verticesNum == firstverticiesNum; }
    string getName() const { return name; }

    //Operation of the vertex.
    EmbeddedVertex* addVertex();
    void deleteVertex();

    //Operation of the edge.
    void insertEdges(EmbeddedEdge* _start, vector<EmbeddedEdge*> _insertEdges);
    void insertEdge(EmbeddedEdge* _start, EmbeddedEdge* _insertEdge);
    void removeEdges(EmbeddedEdge* _start, EmbeddedEdge* _end, EmbeddedEdge** _removedEdges);
    void removeEdges(EmbeddedEdge* _start, int _count, EmbeddedEdge** _removedEdges);
    void removeEdge(EmbeddedEdge* _ed);
    EmbeddedEdge* makeEdge(int _id1, int _id2, bool _flip);

    //IOOperation of the graph.
    void openGraph(string _filepath);
    void saveGraph(string _filepath);

    //Initialize methods.
    void initContractible();
    void initFace();

    //Methods to view a data of graph.
    void viewClockwiseOrder() const;
    void viewUntiClockwiseOrder() const;

    bool isEmpty() const { return (verticesNum == 0); }

protected:
    EmbeddedVertex* vertices[GraphProperty::MAX_VERTEX];
    int verticesNum;
    int edgesNum;
    bool orientable;
    int genus;
    string name;

    void setRotation(string _rotation);
    string toRotationString() const;

private:
    int firstverticiesNum;
    int initializedVerticesNum;

};

#endif
