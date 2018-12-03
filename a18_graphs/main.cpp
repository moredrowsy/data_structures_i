#include <algorithm>           // min_element()
#include <iostream>            // stream
#include <string>              // string
#include "../include/graph.h"  // GraphMatrix class

int main() {
    std::cout << std::string(80, '-') << std::endl;
    std::cout << "WeightedGraphMatrix" << std::endl;
    std::cout << std::string(80, '-') << std::endl;

    graph::WeightedGraphMatrix<std::string, int> wg;

    wg.add_vertex("Rosemead");
    wg.add_vertex("Pasadena");
    wg.add_vertex("Alhambra");
    wg.add_vertex("Temple City");
    wg.add_vertex("Los Angles");
    wg.add_vertex("Arcadia");

    wg.add_edge(0, 1);
    wg.add_weight(0, 1, 2);
    wg.add_edge(0, 5);
    wg.add_weight(0, 5, 9);
    wg.add_edge(1, 2);
    wg.add_weight(1, 2, 8);
    wg.add_edge(1, 3);
    wg.add_weight(1, 3, 15);
    wg.add_edge(1, 5);
    wg.add_weight(1, 5, 6);
    wg.add_edge(2, 3);
    wg.add_weight(2, 3, 1);
    wg.add_edge(4, 2);
    wg.add_weight(4, 2, 7);
    wg.add_edge(5, 4);
    wg.add_weight(5, 4, 3);

    std::cout << "\nLabels: ";
    wg.print_labels();
    std::cout << std::endl;

    std::cout << "\nAdjacency matrix:\n";
    wg.print_edges();

    std::cout << "\nWeights matrix:\n";
    wg.print_weights();

    std::cout << "\nNeighbors:\n";
    wg.print_neighbors();

    std::cout << "\nHas paths?\n";
    std::cout << "paths of (0, 3)? " << has_path(wg, 0, 3) << std::endl;
    std::cout << "paths of (3, 0)? " << has_path(wg, 3, 0) << std::endl;

    std::cout << "\nDijkstra's Shortest Path:\n";
    std::vector<int> distances, predecessor;
    wg.get_dijkstra(0, distances, predecessor);
    wg.print_paths(0, distances, predecessor);
    std::cout << std::endl << std::endl;

    std::cout << std::string(80, '-') << std::endl;
    std::cout << "WeightedGraphSet" << std::endl;
    std::cout << std::string(80, '-') << std::endl;

    graph::WeightedGraphSet<std::string, int> wg_set;

    wg_set.add_vertex("Rosemead");
    wg_set.add_vertex("Pasadena");
    wg_set.add_vertex("Alhambra");
    wg_set.add_vertex("Temple City");
    wg_set.add_vertex("Los Angles");
    wg_set.add_vertex("Arcadia");

    wg_set.add_edge(0, 1);
    wg_set.add_weight(0, 1, 2);
    wg_set.add_edge(0, 5);
    wg_set.add_weight(0, 5, 9);
    wg_set.add_edge(1, 2);
    wg_set.add_weight(1, 2, 8);
    wg_set.add_edge(1, 3);
    wg_set.add_weight(1, 3, 15);
    wg_set.add_edge(1, 5);
    wg_set.add_weight(1, 5, 6);
    wg_set.add_edge(2, 3);
    wg_set.add_weight(2, 3, 1);
    wg_set.add_edge(4, 2);
    wg_set.add_weight(4, 2, 7);
    wg_set.add_edge(5, 4);
    wg_set.add_weight(5, 4, 3);

    std::cout << "\nLabels: ";
    wg_set.print_labels();
    std::cout << std::endl;

    std::cout << "\nWeights set:\n";
    wg_set.print_weights();

    std::cout << "\nNeighbors:\n";
    wg_set.print_neighbors();

    std::cout << "\nHas paths?\n";
    std::cout << "paths of (0, 3)? " << has_path(wg_set, 0, 3) << std::endl;
    std::cout << "paths of (3, 0)? " << has_path(wg_set, 3, 0) << std::endl;

    std::cout << "\nDijkstra's Shortest Path:\n";
    distances.clear();
    predecessor.clear();
    wg_set.get_dijkstra(0, distances, predecessor);
    wg_set.print_paths(0, distances, predecessor);

    return 0;
}