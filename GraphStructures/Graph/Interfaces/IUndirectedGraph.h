#pragma once
#include "../GraphStructures/Vertex/Interfaces/IVertex.h"
#include "../GraphStructures/Edge/Interfaces/IUndirectedEdge.h"
#include "../DataStructures/MutableSegmentedDeque.h"

template <typename T>
class IUndirectedGraph {
public:
// чисто для графа
    virtual IVertex<T>* addVertex(IVertex<T>* vertex) = 0;

    virtual MutableSegmentedDeque<IVertex<T>*>* getAllVertices() = 0;

    virtual IUndirectedEdge<T>* connectVertices(IVertex<T>* firstVertex, IVertex<T>* secondVertexNumber, const T& weight) = 0;

    virtual int removeVertex(IVertex<T>* vertexNumber) = 0;

    virtual int removeEdge(IVertex<T>* firstVertex, IVertex<T>* secondVertex) = 0;

    virtual int removeEdge(IUndirectedEdge<T>* edge) = 0;

    virtual int setNewWeight(IVertex<T>* firstVertex, IVertex<T>* secondVertex, const T& weight) = 0;

    virtual int setNewWeight(IUndirectedEdge<T>* edge, const T& weight) = 0;

    virtual IUndirectedEdge<T>* findCommonEdge(IVertex<T>* firstVertex, IVertex<T>* secondVertex) = 0;

    virtual MutableSegmentedDeque<IUndirectedEdge<T>*>* getAllConnectedEdges(IVertex<T>* vertex) = 0;
    
    virtual ~IUndirectedGraph() {}
};