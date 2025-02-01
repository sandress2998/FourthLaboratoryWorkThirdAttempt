#pragma once
#include <concepts>
#include "../GraphStructures/Edge/Interfaces/IDirectedEdge.h"

template <typename Edge, typename T>
concept DerivedFromIDirectedEdge = std::derived_from<Edge, IDirectedEdge<T>>;