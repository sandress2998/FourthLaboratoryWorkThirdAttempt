#pragma once
#include <concepts>
#include "../Algorithms/DijkstraAlgorithm/Interfaces/IUndirectedPath.h"

template <typename Node, typename T>
concept DerivedFromIUndirectedNode = std::derived_from<Node, IUndirectedNode<T>>;