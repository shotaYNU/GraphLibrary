#ifndef EmbeddedEdge_hpp
#define EmbeddedEdge_hpp

#include "graph_property.hpp"
#include "embedded_face.hpp"

class EmbeddedEdge {
public:
    //Constructors and a destructor.
    EmbeddedEdge();
    EmbeddedEdge(int _start, int _end);
    ~EmbeddedEdge();

    //Getter methods and setter methods.
    void setStart(int _start) { start = _start; opposite->start = _start; }
    int getStart() const { return start; }
    void setEnd(int _end) { end = _end; opposite->end = _end; }
    int getEnd() const { return end; }
    void setNext(EmbeddedEdge* _edge) { next = _edge; opposite->prev = _edge->opposite; }
    EmbeddedEdge* getNext() const { return next; }
    void setPrev(EmbeddedEdge* _edge) { prev = _edge; opposite->next = _edge->opposite; }
    EmbeddedEdge* getPrev() const { return prev; }
    void setInverse(EmbeddedEdge* _edge, bool _flip) { inverse = !_flip ? _edge : _edge->opposite; opposite->inverse = !_flip ? _edge->opposite : _edge; }
    EmbeddedEdge* getInverse() const { return inverse; }
    void setIsContractible(bool _bl) { isContractible = _bl; opposite->isContractible = _bl; }
    bool getIsContractible() const { return isContractible; }
    void setOpposite(EmbeddedEdge* _edge) { opposite = _edge; }
    EmbeddedEdge* getOpposite() const { return opposite; }
    void setRightFace(EmbeddedFace* _face) { rightFace = _face; }
    EmbeddedFace* getRightFace() const { return rightFace; }

    void mark() { marked = true; }
    void unmark() { marked = false; opposite->marked = false; }
    bool getMark() const { return marked; }
    void setIndex(int _index) { index = _index; opposite->index = -1; }
    int getIndex() { return index; }

protected:
    int start;
    int end;
    EmbeddedEdge *next;
    EmbeddedEdge *prev;
    EmbeddedEdge *inverse;
    EmbeddedEdge *opposite;
    EmbeddedFace *rightFace;
    bool isContractible;
    bool marked;
    int index;

private:

};

#endif
