#ifndef PaneledFace_hpp
#define PaneledFace_hpp

#include "../EmbeddedGraph/embedded_face.hpp"

class PaneledFace : public EmbeddedFace  {
public:
    //Constructors and a destructor.
    PaneledFace();
    ~PaneledFace();

    void setPanel() { isPanel = true; }
    void resetPanel() { isPanel = false; }
    bool getPanel() const { return isPanel; }

protected:

private:
    bool isPanel;

};

#endif
