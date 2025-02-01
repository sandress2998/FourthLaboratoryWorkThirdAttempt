#pragma once
#include "../GraphStructures/Vertex/Interfaces/IVertex.h"

template <typename T>
class VertexImpl: public IVertex<T> {
public:
    VertexImpl() = default;

    ~VertexImpl() override {}
};

