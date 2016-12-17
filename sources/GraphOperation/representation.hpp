#ifndef Representation_hpp
#define Representation_hpp

#include "breadth_first_search.hpp"

class Representation {
public:
    enum Results {
        FAIL = -1,
        AUTOMORPHISM = 0,
        BETTER = 1,
    };

    EmbeddedEdge* edge;
    bool clockwise;

    //Constructors and a destructor.
    Representation();
    Representation(const Representation& _obj);
    ~Representation();

    virtual void setRepresentation(EmbeddedEdge* _ed, bool _clockwise);

    //A method to compare representation.
    virtual Results compareRepresentation(const Representation& _rep) const;

    //A method to convert representation to string.
    string toString() const;

protected:
    int representation[GraphProperty::MAX_EDGE];
    int repCount;

private:

};

#endif
