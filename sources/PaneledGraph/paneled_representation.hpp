#ifndef PaneledRepresentation_hpp
#define PaneledRepresentation_hpp

#include "../GraphOperation/representation.hpp"
#include "paneled_face.hpp"
#include "paneled_breadth_first_search.hpp"

class PaneledRepresentation : public Representation {
public:
    //Constructors and a destructor.
    PaneledRepresentation();
    PaneledRepresentation(const PaneledRepresentation& _obj);
    ~PaneledRepresentation();

    void setRepresentation(EmbeddedEdge* _ed, bool _clockwise);
    vector<int> getTraversedVertices() const { return traversedVertices; }

    //A method to compare representation.
    Representation::Results compareRepresentation(const Representation& _rep) const;

protected:
    int representationPanel[GraphProperty::MAX_EDGE];
    vector<int> traversedVertices;

private:

};

#endif
