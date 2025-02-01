#pragma once
#include "../GraphStructures/Edge/Interfaces/IDirectedEdge.h"
#include "../GraphStructures/Vertex/Interfaces/IVertex.h"

template <typename T>
class DirectedEdgeImpl: public IDirectedEdge<T> {
public:
    // дефолтный конструктор только для дека
    DirectedEdgeImpl() = default; 

    DirectedEdgeImpl(IVertex<T>* firstVertex, IVertex<T>* secondVertex, const T& weight): 
        IDirectedEdge<T>(firstVertex, secondVertex, weight) {} 

    // по идее, нигде не должно пригодиться, кроме как в деке 
    DirectedEdgeImpl<T>& operator=(IDirectedEdge<T>& otherEdge) override {
        if (this != &otherEdge) {
            delete this->vertices; 

            this->vertices = otherEdge.getVertices(); // Копируем указатель
            this->weight = otherEdge.getWeight(); // Копируем вес
            return *this;
        }
        return *this;
    }

    const T& getWeight() const override {
        return this->weight;
    }

    void setWeight(const T& newWeight) override {
        this->weight = newWeight;
    }

    Pair<IVertex<T>*>* getVertices() override {
        return this->vertices;
    }
};