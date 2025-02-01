#pragma once

#include <limits>

#include "../DataStructures/MutableSegmentedDeque.h"

#include "../GraphStructures/Edge/Interfaces/IUndirectedEdge.h"
#include "../GraphStructures/Vertex/Interfaces/IVertex.h"
#include "../GraphStructures/Graph/Interfaces/IUndirectedGraph.h"
#include "../Algorithms/DijkstraAlgorithm/Interfaces/IUndirectedPath.h"

#include "../Algorithms/DijkstraAlgorithm/Concepts/UndirectedNodeConcept.h"

template <typename T>
class UndirectedNodeImpl: public IUndirectedNode<T> {
public:
    UndirectedNodeImpl(IVertex<T>* vertex): IUndirectedNode<T>(vertex) {}

    UndirectedNodeImpl(UndirectedNodeImpl<T>* node) {
        this->minPath = node->getMinPath();
        this->used = node->isUsed();
        this->minWeight = node->getMinWeight();
        this->vertex = node->getVertex();
    }

    UndirectedNodeImpl(IUndirectedNode<T>* node) {
        this->minPath = node->getMinPath();
        this->used = node->isUsed();
        this->minWeight = node->getMinWeight();
        this->vertex = node->getVertex();
    }

    bool isUsed() const override {
        return this->used;
    }

    void setUsed() override {
        this->used = true;
    }

    double getMinWeight() const override {
        return this->minWeight;
    }

    void setMinWeight(double newMinWeight) override {
        this->minWeight = newMinWeight;
    }

    IVertex<T>* getVertex() override {
        return this->vertex;
    }

    void setMinPath(IUndirectedNode<T>* nodeToCopyMinPath, IUndirectedEdge<T>* edgeToAddInMinPath) override {
        this->minPath = *(nodeToCopyMinPath->getMinPath());
        this->minPath.append(edgeToAddInMinPath);
    }
};

template <typename T, template <typename> typename Node>
requires DerivedFromIUndirectedNode<Node<T>, T>
class UndirectedPathImpl: public IUndirectedPath<T> {
private:
    IUndirectedNode<T>* findNodeByVertex(MutableSegmentedDeque<IUndirectedNode<T>*>* nodes, IVertex<T>* vertex) {
        for (int i = 0; i < nodes->getLength(); ++i) {
            if (vertex == (*nodes)[i]->getVertex()) {
                return (*nodes)[i];
            }
        }
        throw std::invalid_argument("There is no corresponding node.");
    }

    IUndirectedNode<T>* findNodeByNumber(MutableSegmentedDeque<IUndirectedNode<T>*>* nodes, int number) {
        for (int i = 0; i < nodes->getLength(); ++i) {
            if (number == nodes->get(i)->getNumber()) {
                return (*nodes)[i];
            }
        }    
        throw std::invalid_argument("There is no corresponding node.");
    }
public:
    MutableSegmentedDeque<IUndirectedEdge<T>*>* findShortestPath(IUndirectedGraph<T>* graph, IVertex<T>* vertexFrom, IVertex<T>* vertexTo) override {
        MutableSegmentedDeque<IVertex<T>*>* allVertices = graph->getAllVertices();
        MutableSegmentedDeque<IUndirectedNode<T>*> allNodes;

        for (int i = 0; i < allVertices->getLength(); ++i) {
            allNodes.append(new Node<T>((*allVertices)[i]));
        }

        IUndirectedNode<T>* currentNode = findNodeByVertex(&allNodes, vertexFrom);
        IUndirectedNode<T>* finishNode = findNodeByVertex(&allNodes, vertexTo);

        // ДОБАВЛЕНО
        currentNode->setMinWeight(0);
        
        MutableSegmentedDeque<IUndirectedEdge<T>*>* allConnectedEdges;
        int quantityOfSubmittedVertices = 0;

        while (true) {
            allConnectedEdges = graph->getAllConnectedEdges(currentNode->getVertex());

            IUndirectedNode<T>* connectedNode;
            IUndirectedEdge<T>* connectingEdge;

            for (int i = 0; i < allConnectedEdges->getLength(); ++i) {
                connectingEdge = (*allConnectedEdges)[i];
                if (connectingEdge->getVertices()->getFirst() == currentNode->getVertex()) {
                    connectedNode = findNodeByVertex(&allNodes, connectingEdge->getVertices()->getSecond());
                } else {
                    connectedNode = findNodeByVertex(&allNodes, connectingEdge->getVertices()->getFirst());
                }

                if (connectedNode->isUsed()) {
                    continue;
                }
                
                if (connectedNode->getMinWeight() > currentNode->getMinWeight() + connectingEdge->getWeight()) {
                    connectedNode->setMinWeight(currentNode->getMinWeight() + connectingEdge->getWeight());
                    connectedNode->setMinPath(currentNode, connectingEdge);
                }
            }

            currentNode->setUsed();
            if (++quantityOfSubmittedVertices == allNodes.getLength()) {
                break;
            }
            
            double minShortestPath = INT_MAX;
            for (int i = 0; i < allVertices->getLength(); ++i) {
                if (allNodes[i]->getMinWeight() < minShortestPath && !(allNodes[i]->isUsed())) {
                    currentNode = allNodes[i];
                    minShortestPath = currentNode->getMinWeight();
                }
            }
        }

        MutableSegmentedDeque<IUndirectedEdge<T>*>* shortestPath = new MutableSegmentedDeque<IUndirectedEdge<T>*>();
        *shortestPath = *(finishNode->getMinPath());

        for (int i = 0; i < allNodes.getLength(); ++i) {
            delete allNodes[i];
        }

        return shortestPath;
    }

    double extractMinWeight(MutableSegmentedDeque<IUndirectedEdge<T>*>* minPath) override {
        double minWeight = 0;
        for (int i = 0; i < minPath->getLength(); ++i) {
            minWeight += minPath->get(i)->getWeight();
        }
        if (minPath->getLength() == 0) {
            return INT_MAX;
        }
        return minWeight;
    }
};
