#pragma once
#include "../GraphStructures/Vertex/Interfaces/IVertex.h"
#include "../GraphStructures/Edge/Interfaces/IDirectedEdge.h"
#include "../DataStructures/MutableSegmentedDeque.h"

template <typename T>
class IDirectedGraph {
public:
// чисто для графа
    virtual IVertex<T>* addVertex(IVertex<T>* vertex) = 0;

    virtual MutableSegmentedDeque<IVertex<T>*>* getAllVertices() = 0;

    virtual IDirectedEdge<T>* connectVertices(IVertex<T>* firstVertex, IVertex<T>* secondVertexNumber, const T& weight) = 0;

    virtual int removeVertex(IVertex<T>* vertexNumber) = 0;

    virtual int removeEdge(IVertex<T>* firstVertex, IVertex<T>* secondVertex) = 0;

    virtual int removeEdge(IDirectedEdge<T>* edge) = 0;

    virtual int setNewWeight(IVertex<T>* firstVertex, IVertex<T>* secondVertex, const T& weight) = 0;

    virtual int setNewWeight(IDirectedEdge<T>* edge, const T& weight) = 0;

    virtual IDirectedEdge<T>* findCommonEdge(IVertex<T>* firstVertex, IVertex<T>* secondVertex) = 0;
    
    virtual MutableSegmentedDeque<IDirectedEdge<T>*>* getAllOutgoingEdges(IVertex<T>* vertex) = 0;

    virtual MutableSegmentedDeque<IDirectedEdge<T>*>* getAllIngoingEdges(IVertex<T>* vertex) = 0;

    virtual ~IDirectedGraph() {}
};