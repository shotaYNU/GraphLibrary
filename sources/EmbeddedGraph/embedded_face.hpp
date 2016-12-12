#ifndef EmbeddedFace_hpp
#define EmbeddedFace_hpp

#include "graph_property.hpp"

class EmbeddedFace {
public:
    //Constructors and a destructor.
    EmbeddedFace();
    EmbeddedFace(int _num);
    ~EmbeddedFace();

    //Getter methods and setter methods.
    void setFaceVerticesNum(int _num) { faceVerticesNum = _num; }
    int getFaceVerticesNum() const { return faceVerticesNum; }
    void setFace(long long _f) { face = _f; }
    long long getFace() const { return face; }

protected:
    int faceVerticesNum;
    long long face;

private:

};

#endif
