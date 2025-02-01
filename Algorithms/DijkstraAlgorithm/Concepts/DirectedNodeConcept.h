#pragma once
#include <concepts>
#include "../Algorithms/DijkstraAlgorithm/Interfaces/IDirectedPath.h"

template <typename Node, typename T>
concept DerivedFromIDirectedNode = std::derived_from<Node, IDirectedNode<T>>;