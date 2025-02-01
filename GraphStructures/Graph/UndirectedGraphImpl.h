#pragma once

#include <stdexcept>

#include "../DataStructures/MutableSegmentedDeque.h"
#include "../GraphStructures/Graph/Interfaces/IUndirectedGraph.h"
#include "../GraphStructures/Vertex/Interfaces/IVertex.h"
#include "../GraphStructures/Edge/Interfaces/IUndirectedEdge.h"

#include "../GraphStructures/Edge/Concepts/UndirectedEdgeConcept.h"
#include "../GraphStructures/Vertex/Concepts/VertexConcept.h"

template <typename T, template <typename> typename Vertex, template <typename> typename Edge>
requires DerivedFromIVertex<Vertex<T>, T> && DerivedFromIUndirectedEdge<Edge<T>, T>
class UndirectedGraphImpl: public IUndirectedGraph<T> {
private:
    class Node {
        public:
            Node() = default;

            Node(IVertex<T>* vertex): vertex(vertex) {}

            IVertex<T>* getVertex() {
                return vertex;
            }

            IUndirectedEdge<T>* findCommonEdge(Node* anotherNode) {
                // подразумевается, что this - вершина, из которой выходит ребро.
                Pair<IVertex<T>*>* verticesInEdge;
                for (int i = 0; i < edges.getLength(); ++i) {
                    verticesInEdge = (edges[i])->getVertices();
                    if (verticesInEdge->getSecond() == anotherNode->getVertex() || verticesInEdge->getFirst() == anotherNode->getVertex()) {
                        return edges[i];
                    }
                }
                return nullptr;
            }

            // мы должны быть уверены, что у нас нет 
            IUndirectedEdge<T>* addEdge(IUndirectedEdge<T>* edge) {
                edges.append(edge);
                return edge;
            }

            MutableSegmentedDeque<IUndirectedEdge<T>*>* getAllEdges() {
                return &edges;
            }

            int removeEdge(IUndirectedEdge<T>* edgeToRemove) {
                for (int i = 0; i < edges.getLength(); ++i) {
                    if (edgeToRemove == edges[i]) {
                        edges.removeAt(i);
                        return 0;
                    }
                }
                return 1;
            }

            ~Node() {}

        private:
            IVertex<T>* vertex;
            MutableSegmentedDeque<IUndirectedEdge<T>*> edges;
    };

    MutableSegmentedDeque<Node> allNodes;

    Node* findNodeByVertex(IVertex<T>* vertex) {
        for (int i = 0; i < allNodes.getLength(); ++i) {
            if (vertex == allNodes[i].getVertex()) {
                return &(allNodes[i]);
            }
        }
        return nullptr;
    }

public:
    IVertex<T>* addVertex(IVertex<T>* vertexToAdd) override {
        if (findNodeByVertex(vertexToAdd)) {
            return nullptr;
        }
        allNodes.append(Node(vertexToAdd));
        return vertexToAdd;
    }

    IUndirectedEdge<T>* connectVertices(IVertex<T>* firstVertex, IVertex<T>* secondVertex, const T& weight) override {
        Node* firstNode = findNodeByVertex(firstVertex);
        Node* secondNode = findNodeByVertex(secondVertex);

        if (!firstNode->findCommonEdge(secondNode)) {
            IUndirectedEdge<T>* newEdge = new Edge(firstVertex, secondVertex, weight);
            firstNode->addEdge(newEdge);
            secondNode->addEdge(newEdge);
            return newEdge;
        } else {
            return nullptr;
        }
    }

    int removeVertex(IVertex<T>* vertex) override {
        Node* nodeToRemove = findNodeByVertex(vertex);
        for (int i = 0; i < allNodes.getLength(); ++i) {
            if (&(allNodes[i]) == nodeToRemove) {
                MutableSegmentedDeque<IUndirectedEdge<T>*>* edgesToRemove = nodeToRemove->getAllEdges();
                for (int j = 0; j < edgesToRemove->getLength(); ++j) {
                    if ((*edgesToRemove)[j]->getVertices()->getSecond() != vertex) {
                        findNodeByVertex((*edgesToRemove)[j]->getVertices()->getSecond())->removeEdge((*edgesToRemove)[j]);
                    } else {
                        findNodeByVertex((*edgesToRemove)[j]->getVertices()->getFirst())->removeEdge((*edgesToRemove)[j]);
                    }
                }
                allNodes.removeAt(i);
                return 0;
            }
        }
        return 1;
    }

    int removeEdge(IVertex<T>* firstVertex, IVertex<T>* secondVertex) override {
        Node* firstNodeThatContainsEdge = findNodeByVertex(firstVertex);
        Node* secondNodeThatContainsEdge = findNodeByVertex(secondVertex);
        IUndirectedEdge<T>* edgeToRemove = firstNodeThatContainsEdge->findCommonEdge(secondNodeThatContainsEdge);
        if (edgeToRemove == nullptr) {
            return 1;
        }
        firstNodeThatContainsEdge->removeEdge(edgeToRemove);
        secondNodeThatContainsEdge->removeEdge(edgeToRemove);
        delete edgeToRemove;
        return 0;
    }

    int removeEdge(IUndirectedEdge<T>* edgeToRemove) override {
        if (edgeToRemove == nullptr) {
            return 1;
        }
        Node* firstNodeThatContainsEdge = findNodeByVertex(edgeToRemove->getVertices()->getFirst());
        Node* secondNodeThatContainsEdge = findNodeByVertex(edgeToRemove->getVertices()->getSecond());

        if (!firstNodeThatContainsEdge || !secondNodeThatContainsEdge) {
            return 2; // в этом случае мы передали в параметры ребро, которое не принадлежит графу
        }

        firstNodeThatContainsEdge->removeEdge(edgeToRemove);
        secondNodeThatContainsEdge->removeEdge(edgeToRemove);
        delete edgeToRemove;
        return 0;
    }

    int setNewWeight(IVertex<T>* firstVertex, IVertex<T>* secondVertex, const T& weight) override {
        Node* firstNodeThatContainsEdge = findNodeByVertex(firstVertex);
        Node* secondNodeThatContainsEdge = findNodeByVertex(secondVertex);

        if (!firstNodeThatContainsEdge || !secondNodeThatContainsEdge) {
            return 2; // в этом случае мы передали в параметры вершины, которое не принадлежат графу
        }

        IUndirectedEdge<T>* edgeToUpdate = firstNodeThatContainsEdge->findCommonEdge(secondNodeThatContainsEdge);
        if (edgeToUpdate == nullptr) {
            return 1; // вершины в данном графе не содержат такого ребра
        }
        edgeToUpdate->setWeight(weight);
        return 0;
    }

    int setNewWeight(IUndirectedEdge<T>* edgeToUpdate, const T& weight) override {
        edgeToUpdate->setWeight(weight);
        return 0;
    }

    MutableSegmentedDeque<IVertex<T>*>* getAllVertices() override {
        MutableSegmentedDeque<IVertex<T>*>* allVertices = new MutableSegmentedDeque<IVertex<T>*>();
        for (int i = 0; i < allNodes.getLength(); ++i) {
            allVertices->append(allNodes[i].getVertex());
        }
        return allVertices;
    }

    IUndirectedEdge<T>* findCommonEdge(IVertex<T>* firstVertex, IVertex<T>* secondVertex) override {
        Node* firstNode = findNodeByVertex(firstVertex);
        Node* secondNode = findNodeByVertex(secondVertex);

        IUndirectedEdge<T>* commonEdge = firstNode->findCommonEdge(secondNode);
        if (commonEdge) {
            return commonEdge;
        }
        return nullptr;
    }

    MutableSegmentedDeque<IUndirectedEdge<T>*>* getAllConnectedEdges(IVertex<T>* vertex) override {
        return findNodeByVertex(vertex)->getAllEdges();
    }

    ~UndirectedGraphImpl() override {
        MutableSegmentedDeque<IUndirectedEdge<T>*>* allEdges;
        for (int i = 0; i < allNodes.getLength(); ++i) {
            allEdges = allNodes[i].getAllEdges();
            for (int j = 0; j < allEdges->getLength(); ++j) {
                removeEdge((*allEdges)[j]);
            }
        }
    } // насчет Edge переживать не стоит, они все удалятся при удалении allNodes
};