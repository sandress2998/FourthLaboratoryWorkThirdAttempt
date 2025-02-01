#pragma once
#include "../GraphStructures/Edge/Interfaces/IUndirectedEdge.h"
#include "../GraphStructures/Vertex/Interfaces/IVertex.h"

template <typename T>
class UndirectedEdgeImpl: public IUndirectedEdge<T> {
public:
    // дефолтный конструктор только для дека
    UndirectedEdgeImpl() = default; 

    UndirectedEdgeImpl(IVertex<T>* firstVertex, IVertex<T>* secondVertex, const T& weight): 
        IUndirectedEdge<T>(firstVertex, secondVertex, weight) {} 

    // по идее, нигде не должно пригодиться, кроме как в деке 
    UndirectedEdgeImpl<T>& operator=(IUndirectedEdge<T>& otherEdge) override {
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