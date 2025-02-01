#pragma once

#include <limits>
#include "../DataStructures/MutableSegmentedDeque.h"

#include "../GraphStructures/Vertex/Interfaces/IVertex.h"
#include "../GraphStructures/Graph/Interfaces/IUndirectedGraph.h"
#include "../Algorithms/DijkstraAlgorithm/Interfaces/IUndirectedPath.h"
#include "../GraphStructures/Edge/Interfaces/IUndirectedEdge.h"

template <typename T>
class IUndirectedNode {
protected:
    IVertex<T>* vertex;

    MutableSegmentedDeque<IUndirectedEdge<T>*> minPath;

    double minWeight = INT_MAX;
    
    bool used = false;
    
public:
    IUndirectedNode() = default;

    IUndirectedNode(IVertex<T>* vertex): vertex(vertex) {}

    virtual bool isUsed() const = 0;

    virtual void setUsed() = 0;

    virtual double getMinWeight() const = 0;

    virtual void setMinWeight(double newWeight) = 0;

    MutableSegmentedDeque<IUndirectedEdge<T>*>* getMinPath() {
        return &minPath;
    }    

    virtual void setMinPath(IUndirectedNode<T>* nodeToCopyMinPath, IUndirectedEdge<T>* edgeToAddInMinPath) = 0;

    virtual IVertex<T>* getVertex() = 0;
};

template <typename T>
class IUndirectedPath {
public:
    virtual MutableSegmentedDeque<IUndirectedEdge<T>*>* findShortestPath(IUndirectedGraph<T>* inputGraph, IVertex<T>* vertexFrom, IVertex<T>* vertexTo) = 0;

    virtual double extractMinWeight(MutableSegmentedDeque<IUndirectedEdge<T>*>* minPath) = 0;
};