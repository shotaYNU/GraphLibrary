#ifndef PaneledRepresentation_hpp
#define PaneledRepresentation_hpp

#include "../GraphOperation/representation.hpp"
#include "paneled_face.hpp"

class PaneledRepresentation : public Representation {
public:
    //Constructors and a destructor.
    PaneledRepresentation();
    PaneledRepresentation(const PaneledRepresentation& _obj);
    ~PaneledRepresentation();

    void setRepresentation(EmbeddedEdge* _ed, bool _clockwise);

    //A method to compare representation.
    Representation::Results compareRepresentation(const Representation& _rep) const;

protected:
    int representationPanel[GraphProperty::MAX_EDGE];

private:

};

#endif