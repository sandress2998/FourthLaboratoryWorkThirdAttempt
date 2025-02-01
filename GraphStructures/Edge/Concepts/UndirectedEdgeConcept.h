#pragma once
#include <concepts>
#include "../GraphStructures/Edge/Interfaces/IUndirectedEdge.h"

template <typename Edge, typename T>
concept DerivedFromIUndirectedEdge = std::derived_from<Edge, IUndirectedEdge<T>>;