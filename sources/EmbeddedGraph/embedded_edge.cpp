#include "embedded_edge.hpp"

EmbeddedEdge::EmbeddedEdge()
{
    start = -1;
    end = -1;
    next = nullptr;
    prev = nullptr;
    inverse = nullptr;
    opposite = nullptr;
    rightFace = nullptr;
    isContractible = false;
    marked = false;
    index = 0;
}

EmbeddedEdge::EmbeddedEdge(int _start, int _end)
{
    start = _start;
    end = _end;
    next = nullptr;
    prev = nullptr;
    inverse = nullptr;
    opposite = nullptr;
    rightFace = nullptr;
    isContractible = false;
    marked = false;
    index = 0;
}

EmbeddedEdge::~EmbeddedEdge()
{
}
