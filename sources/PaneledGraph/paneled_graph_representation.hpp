#ifndef PaneledGraphRepresentation_hpp
#define PaneledGraphRepresentation_hpp

#include "paneled_graph.hpp"
#include "paneled_representation.hpp"
#include "../GraphOperation/graph_representation.hpp"

class PaneledGraphRepresentation : public GraphRepresentation {
public:
    //Constructors and a destructor.
    PaneledGraphRepresentation();
    PaneledGraphRepresentation(PaneledGraph* _graph);
    ~PaneledGraphRepresentation();
    void setBestRepresentation();
    bool getTraversedAll() const { return traversedAll; }
    Representation::Results compareRepresentation(const GraphRepresentation& _graph_rep) const;

protected:
    void setNewBestRepresentation() { bestRepresentation = new PaneledRepresentation(*(PaneledRepresentation*)newRepresentation); }
    bool traversedAll;
    vector<PaneledRepresentation*> multipleBestRepresentations;

private:

};

#endif
