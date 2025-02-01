#pragma once
#include "../GraphStructures/Vertex/Interfaces/IVertex.h"

template <typename Vertex, typename T>
concept DerivedFromIVertex = std::derived_from<Vertex, IVertex<T>>;