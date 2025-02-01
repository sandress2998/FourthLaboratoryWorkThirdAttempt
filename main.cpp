#include "../GraphStructures/Graph/UndirectedGraphImpl.h"
#include "../GraphStructures/Vertex/VertexImpl.h"
#include "../GraphStructures/Edge/UndirectedEdgeImpl.h"

#include "../GraphStructures/Graph/DirectedGraphImpl.h"
#include "../GraphStructures/Vertex/VertexImpl.h"
#include "../GraphStructures/Edge/DirectedEdgeImpl.h"

#include "../Algorithms/DijkstraAlgorithm/DirectedPathImpl.h"
#include "../Algorithms/DijkstraAlgorithm/UndirectedPathImpl.h"

#include "../GraphStructures/Edge/TemplatesForEdge/Road.h"

int main() {
    UndirectedGraphImpl<Road, VertexImpl, UndirectedEdgeImpl> graph; 

    //UndirectedGraphImpl<Road, VertexImpl, UndirectedEdgeImpl> graph; 
    VertexImpl<Road> vertex1;
    VertexImpl<Road> vertex2;
    VertexImpl<Road> vertex3;
    VertexImpl<Road> vertex4;

    graph.addVertex(&vertex1);
    graph.addVertex(&vertex2);
    graph.addVertex(&vertex3);
    graph.addVertex(&vertex4);

    graph.connectVertices(&vertex1, &vertex2, 25);
    graph.connectVertices(&vertex3, &vertex2, 5);
    graph.connectVertices(&vertex4, &vertex3, 12);
    graph.connectVertices(&vertex4, &vertex1, 4);

    UndirectedPathImpl<Road, UndirectedNodeImpl> path;
    MutableSegmentedDeque<IUndirectedEdge<Road>*>* shortestPath = path.findShortestPath(&graph, &vertex2, &vertex1);
    std::cout << shortestPath->getLength() << "\n";
    std::cout << path.extractMinWeight(shortestPath);

    return 0;
}