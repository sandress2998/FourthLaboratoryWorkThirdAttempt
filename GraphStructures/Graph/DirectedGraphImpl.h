#pragma once

#include <stdexcept>

#include "../DataStructures/MutableSegmentedDeque.h"
#include "../GraphStructures/Graph/Interfaces/IDirectedGraph.h"
#include "../GraphStructures/Vertex/Interfaces/IVertex.h"
#include "../GraphStructures/Edge/Interfaces/IDirectedEdge.h"

#include "../GraphStructures/Edge/Concepts/DirectedEdgeConcept.h"
#include "../GraphStructures/Vertex/Concepts/VertexConcept.h"

template <typename T, template <typename> typename Vertex, template <typename> typename Edge>
requires DerivedFromIVertex<Vertex<T>, T> && DerivedFromIDirectedEdge<Edge<T>, T>
class DirectedGraphImpl: public IDirectedGraph<T> {
private:
    class Node {
        public:
            Node() = default;

            Node(IVertex<T>* vertex): vertex(vertex) {}

            IVertex<T>* getVertex() {
                return vertex;
            }

            IDirectedEdge<T>* findCommonEdge(Node* anotherNode) {
                // подразумевается, что this - вершина, из которой выходит ребро.
                Pair<IVertex<T>*>* verticesInEdge;
                for (int i = 0; i < outgoingEdges.getLength(); ++i) {
                    verticesInEdge = (outgoingEdges[i])->getVertices();
                    if (verticesInEdge->getSecond() == anotherNode->getVertex()) {
                        return outgoingEdges[i];
                    }
                }
                return nullptr;
            }

            IDirectedEdge<T>* addOutgoingEdge(IDirectedEdge<T>* edge) {
                outgoingEdges.append(edge);
                return edge;
            }

            IDirectedEdge<T>* addIngoingEdge(IDirectedEdge<T>* edge) {
                ingoingEdges.append(edge);
                return edge;
            }

            MutableSegmentedDeque<IDirectedEdge<T>*>* getAllOutgoingEdges() {
                return &outgoingEdges;
            }

            MutableSegmentedDeque<IDirectedEdge<T>*>* getAllIngoingEdges() {
                return &ingoingEdges;
            }

            void removeEdge(IDirectedEdge<T>* edgeToRemove) {
                for (int i = 0; i < outgoingEdges.getLength(); ++i) {
                    if (edgeToRemove == outgoingEdges[i]) {
                        outgoingEdges.removeAt(i);
                    }
                }
                
                for (int i = 0; i < ingoingEdges.getLength(); ++i) {
                    if (edgeToRemove == ingoingEdges[i]) {
                        ingoingEdges.removeAt(i);
                        return;
                    }
                }
            }

            // так выходит, что мы очищаем Node от ребер вне деструктора ~Node
            ~Node() {}

        private:
            IVertex<T>* vertex;
            MutableSegmentedDeque<IDirectedEdge<T>*> outgoingEdges;
            MutableSegmentedDeque<IDirectedEdge<T>*> ingoingEdges;
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

    IDirectedEdge<T>* connectVertices(IVertex<T>* firstVertex, IVertex<T>* secondVertex, const T& weight) override {
        /*
        if (firstVertex == secondVertex) {
            throw std::invalid_argument("You tried to connect the one and the same vertexes.\n");
        }
        */
        Node* firstNode = findNodeByVertex(firstVertex);
        Node* secondNode = findNodeByVertex(secondVertex);

        if (!firstNode->findCommonEdge(secondNode)) {
            IDirectedEdge<T>* newEdge = new Edge(firstVertex, secondVertex, weight);
            firstNode->addOutgoingEdge(newEdge);
            secondNode->addIngoingEdge(newEdge);
            return newEdge;
        } else {
            return nullptr;
        }
    }

    int removeVertex(IVertex<T>* vertex) override {
        Node* nodeToRemove = findNodeByVertex(vertex);
        for (int i = 0; i < allNodes.getLength(); ++i) {
            if (&(allNodes[i]) == nodeToRemove) {
                MutableSegmentedDeque<IDirectedEdge<T>*>* outgoingEdgesToRemove = nodeToRemove->getAllOutgoingEdges();
                for (int j = 0; j < outgoingEdgesToRemove->getLength(); ++j) {
                    findNodeByVertex((*outgoingEdgesToRemove)[j]->getVertices()->getSecond())->removeEdge((*outgoingEdgesToRemove)[j]);
                }
                MutableSegmentedDeque<IDirectedEdge<T>*>* ingoingEdgesToRemove = nodeToRemove->getAllIngoingEdges();
                for (int j = 0; j < ingoingEdgesToRemove->getLength(); ++j) {
                    findNodeByVertex((*ingoingEdgesToRemove)[j]->getVertices()->getFirst())->removeEdge((*ingoingEdgesToRemove)[j]);
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
        IDirectedEdge<T>* edgeToRemove = firstNodeThatContainsEdge->findCommonEdge(secondNodeThatContainsEdge);
        if (edgeToRemove == nullptr) {
            return 1;
        }
        firstNodeThatContainsEdge->removeEdge(edgeToRemove);
        secondNodeThatContainsEdge->removeEdge(edgeToRemove);
        delete edgeToRemove;
        return 0;
    }

    int removeEdge(IDirectedEdge<T>* edgeToRemove) override {
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

        IDirectedEdge<T>* edgeToUpdate = firstNodeThatContainsEdge->findCommonEdge(secondNodeThatContainsEdge);
        if (edgeToUpdate == nullptr) {
            return 1; // вершины в данном графе не содержат такого ребра
        }
        edgeToUpdate->setWeight(weight);
        return 0;
    }

    int setNewWeight(IDirectedEdge<T>* edgeToUpdate, const T& weight) override {
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

    IDirectedEdge<T>* findCommonEdge(IVertex<T>* firstVertex, IVertex<T>* secondVertex) override {
        Node* firstNode = findNodeByVertex(firstVertex);
        Node* secondNode = findNodeByVertex(secondVertex);

        IDirectedEdge<T>* commonEdge = firstNode->findCommonEdge(secondNode);
        if (commonEdge) {
            return commonEdge;
        }
        return nullptr;
    }

    MutableSegmentedDeque<IDirectedEdge<T>*>* getAllOutgoingEdges(IVertex<T>* vertex) override {
        return findNodeByVertex(vertex)->getAllOutgoingEdges();
    }

        MutableSegmentedDeque<IDirectedEdge<T>*>* getAllIngoingEdges(IVertex<T>* vertex) override {
        return findNodeByVertex(vertex)->getAllIngoingEdges();
    }

    ~DirectedGraphImpl() override {
        MutableSegmentedDeque<IDirectedEdge<T>*>* allEdges;
        for (int i = 0; i < allNodes.getLength(); ++i) {
            allEdges = allNodes[i].getAllOutgoingEdges();
            for (int j = 0; j < allEdges->getLength(); ++j) {
                removeEdge((*allEdges)[j]);
            }
        }
    } // насчет Edge переживать не стоит, они все удалятся при удалении allNodes
};