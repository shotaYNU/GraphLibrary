#ifndef EmbeddedFace_hpp
#define EmbeddedFace_hpp

#include "graph_property.hpp"

class EmbeddedFace {
public:
    //Constructors and a destructor.
    EmbeddedFace();
    ~EmbeddedFace();

    //Getter methods and setter methods.
    void setFaceVertex(int _vertex) { face.push_back(_vertex); }
    void resetFaceVertex(int _vertex) { face.erase(remove(face.begin(), face.end(), _vertex), face.end()); }
    vector<int> getFace() const { return face; }

protected:
    vector<int> face;

private:

};

#endif
