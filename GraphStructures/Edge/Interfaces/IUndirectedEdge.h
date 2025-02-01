#pragma once
#include "../GraphStructures/Vertex/Interfaces/IVertex.h"
#include "../GraphStructures/Edge/Pair.h"

template <typename T>
class IUndirectedEdge {
protected:
    Pair<IVertex<T>*>* vertices;
    T weight;

public:
    IUndirectedEdge(IVertex<T>* firstVertex, IVertex<T>* secondVertex, const T& weight): 
        vertices(new Pair<IVertex<T>*>(firstVertex, secondVertex)), weight(weight) {}

    virtual const T& getWeight() const = 0;

    virtual void setWeight(const T& newWeight) = 0;

    virtual Pair<IVertex<T>*>* getVertices() = 0;

    virtual IUndirectedEdge<T>& operator=(IUndirectedEdge<T>& other) = 0;

    virtual ~IUndirectedEdge() {
        delete vertices;
    }
};