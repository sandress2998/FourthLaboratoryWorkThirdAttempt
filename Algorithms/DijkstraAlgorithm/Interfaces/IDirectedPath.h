#pragma once

#include "../DataStructures/MutableSegmentedDeque.h"

#include "../GraphStructures/Vertex/Interfaces/IVertex.h"
#include "../GraphStructures/Graph/Interfaces/IUndirectedGraph.h"
#include "../Algorithms/DijkstraAlgorithm/Interfaces/IDirectedPath.h"
#include "../GraphStructures/Edge/Interfaces/IDirectedEdge.h"

template <typename T>
class IDirectedNode {
protected:
    IVertex<T>* vertex;

    MutableSegmentedDeque<IDirectedEdge<T>*> minPath;

    double minWeight = INT_MAX;
    
    bool used = false;

public:
    IDirectedNode() = default;

    IDirectedNode(IVertex<T>* vertex): vertex(vertex) {}

    virtual bool isUsed() const = 0;

    virtual void setUsed() = 0;

    virtual double getMinWeight() const = 0;

    virtual void setMinWeight(double newWeight) = 0;

    MutableSegmentedDeque<IDirectedEdge<T>*>* getMinPath() {
        return &minPath;
    }

    virtual void setMinPath(IDirectedNode<T>* nodeToCopyMinPath, IDirectedEdge<T>* edgeToAddInMinPath) = 0;

    virtual IVertex<T>* getVertex() = 0;
};

template <typename T>
class IDirectedPath {
public:
    virtual MutableSegmentedDeque<IDirectedEdge<T>*>* findShortestPath(IDirectedGraph<T>* inputGraph, IVertex<T>* vertexFrom, IVertex<T>* vertexTo) = 0;

    virtual double extractMinWeight(MutableSegmentedDeque<IDirectedEdge<T>*>* minPath) = 0;
};