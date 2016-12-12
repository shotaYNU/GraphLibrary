#include "embedded_face.hpp"

EmbeddedFace::EmbeddedFace()
{
    faceVerticesNum = 0;
    face = 0ULL;
}

EmbeddedFace::EmbeddedFace(int _num)
{
    faceVerticesNum = _num;
    face = 0ULL;
}

EmbeddedFace::~EmbeddedFace()
{
}
