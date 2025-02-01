#pragma once

#include <limits>

#include "../DataStructures/MutableSegmentedDeque.h"

#include "../GraphStructures/Edge/Interfaces/IDirectedEdge.h"
#include "../GraphStructures/Vertex/Interfaces/IVertex.h"
#include "../GraphStructures/Graph/Interfaces/IDirectedGraph.h"
#include "../Algorithms/DijkstraAlgorithm/Interfaces/IDirectedPath.h"

#include "../Algorithms/DijkstraAlgorithm/Concepts/DirectedNodeConcept.h"

template <typename T>
class DirectedNodeImpl: public IDirectedNode<T> {
public:
    DirectedNodeImpl(IVertex<T>* vertex): IDirectedNode<T>(vertex) {}

    DirectedNodeImpl(DirectedNodeImpl<T>* node) {
        this->minPath = node->getMinPath();
        this->used = node->isUsed();
        this->minWeight = node->getMinWeight();
        this->vertex = node->getVertex();
    }

    DirectedNodeImpl(IDirectedNode<T>* node) {
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

    void setMinPath(IDirectedNode<T>* nodeToCopyMinPath, IDirectedEdge<T>* edgeToAddInMinPath) override {
        this->minPath = *(nodeToCopyMinPath->getMinPath());
        this->minPath.append(edgeToAddInMinPath);
    }
};

template <typename T, template <typename> typename Node>
requires DerivedFromIDirectedNode<Node<T>, T>
class DirectedPathImpl: public IDirectedPath<T> {
private:
    IDirectedNode<T>* findNodeByVertex(MutableSegmentedDeque<IDirectedNode<T>*>* nodes, IVertex<T>* vertex) {
        for (int i = 0; i < nodes->getLength(); ++i) {
            if (vertex == (*nodes)[i]->getVertex()) {
                return (*nodes)[i];
            }
        }    
        throw std::invalid_argument("There is no corresponding node.");
    }

public:
//по идее, если запихать в graph любой ориентированный граф, то все будет работать.
    MutableSegmentedDeque<IDirectedEdge<T>*>* findShortestPath(IDirectedGraph<T>* graph, IVertex<T>* vertexFrom, IVertex<T>* vertexTo) override {
        MutableSegmentedDeque<IVertex<T>*>* allVertices = graph->getAllVertices();
        MutableSegmentedDeque<IDirectedNode<T>*> allNodes;

        for (int i = 0; i < allVertices->getLength(); ++i) {
            allNodes.append(new Node<T>((*allVertices)[i]));
        }

        IDirectedNode<T>* currentNode = findNodeByVertex(&allNodes, vertexFrom);
        IDirectedNode<T>* finishNode = findNodeByVertex(&allNodes, vertexTo);

        // ДОБАВЛЕНА
        currentNode->setMinWeight(0);
        
        MutableSegmentedDeque<IDirectedEdge<T>*>* allOutgoingEdges;
        int quantityOfSubmittedVertices = 0;

        while (true) {
            allOutgoingEdges = graph->getAllOutgoingEdges(currentNode->getVertex());

            IDirectedNode<T>* connectedNode;
            IDirectedEdge<T>* connectingEdge;

            for (int i = 0; i < allOutgoingEdges->getLength(); ++i) {
                connectingEdge = (*allOutgoingEdges)[i];
                connectedNode = findNodeByVertex(&allNodes, connectingEdge->getVertices()->getSecond());

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

        MutableSegmentedDeque<IDirectedEdge<T>*>* shortestPath = new MutableSegmentedDeque<IDirectedEdge<T>*>();
        *shortestPath = *(finishNode->getMinPath());

        for (int i = 0; i < allNodes.getLength(); ++i) {
            delete allNodes[i];
        }

        return shortestPath;
    }

    double extractMinWeight(MutableSegmentedDeque<IDirectedEdge<T>*>* minPath) override {
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