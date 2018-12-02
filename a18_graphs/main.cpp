#include <iostream>            // stream
#include <string>              // string
#include "../include/graph.h"  // GraphMatrix class

int main() {
    // std::cout << "GraphMatrix\n";

    // graph::GraphMatrix<std::string> g;

    // g.add_vertex("Rosemead");
    // g.add_vertex("Pasadena");
    // g.add_vertex("Alhambra");
    // g.add_vertex("El Monte");

    // g.add_edge(0, 1);
    // g.add_edge(0, 3);
    // g.add_edge(1, 1);
    // g.add_edge(1, 2);
    // g.add_edge(3, 1);
    // g.add_edge(3, 2);
    // g.add_edge(3, 3);

    // std::cout << "Adjacency matrix:\n";
    // g.print_edges();

    // std::cout << "Labels: ";
    // g.print_labels();
    // std::cout << std::endl;

    // std::cout << "Neighbors:\n";
    // for(std::size_t i = 0; i < g.size(); ++i) {
    //     std::cout << i << ": ";

    //     auto set = g.neighbors(i);
    //     for(const auto& a : set) std::cout << g[a] << ' ';

    //     std::cout << std::endl;
    // }

    // std::cout << "Other info:\n";
    // std::cout << "size: " << g.size() << std::endl;
    // std::cout << "is edge at 0, 1? " << g.is_edge(0, 1) << std::endl;
    // std::cout << "is edge at 1, 0? " << g.is_edge(1, 0) << std::endl;

    // std::cout << "\n\nGraphSet:\n";

    // graph::GraphSet<std::string> g_set;

    // g_set.add_vertex("Rosemead");
    // g_set.add_vertex("Pasadena");
    // g_set.add_vertex("Alhambra");
    // g_set.add_vertex("El Monte");

    // g_set.add_edge(0, 1);
    // g_set.add_edge(0, 3);
    // g_set.add_edge(1, 1);
    // g_set.add_edge(1, 2);
    // g_set.add_edge(3, 1);
    // g_set.add_edge(3, 2);
    // g_set.add_edge(3, 3);

    // std::cout << "Neighbors:\n";

    // for(std::size_t i = 0; i < g_set.size(); ++i) {
    //     std::cout << i << ": ";

    //     auto set = g_set.neighbors(i);
    //     for(const auto& a : set) std::cout << g_set[a] << ' ';

    //     std::cout << std::endl;
    // }

    // std::cout << "Other info:\n";
    // std::cout << "size: " << g_set.size() << std::endl;
    // std::cout << "is edge at 0, 1? " << g_set.is_edge(0, 1) << std::endl;
    // std::cout << "is edge at 1, 0? " << g_set.is_edge(1, 0) << std::endl;

    std::cout << "\n\nWeightedGraphMatrix:\n";

    graph::WeightedGraphMatrix<std::string, int> wg;

    wg.add_vertex("Rosemead");
    wg.add_vertex("Pasadena");
    wg.add_vertex("Alhambra");
    wg.add_vertex("El Monte");

    wg.add_edge(0, 1);
    wg.add_weight(0, 1, 100);
    wg.add_edge(0, 3);
    wg.add_edge(1, 1);
    wg.add_edge(1, 2);
    wg.add_edge(3, 1);
    wg.add_edge(3, 2);
    wg.add_edge(3, 3);

    std::cout << "Adjacency matrix:\n";
    wg.print_edges();

    std::cout << "Labels: ";
    wg.print_labels();
    std::cout << std::endl;

    std::cout << "Neighbors:\n";
    for(std::size_t i = 0; i < wg.size(); ++i) {
        std::cout << i << ": ";

        auto set = wg.neighbors(i);
        for(const auto& a : set) std::cout << wg[a] << ' ';

        std::cout << std::endl;
    }

    std::cout << "Weights @ 0, 1: " << wg.weight(0, 1) << std::endl;
    std::cout << "Weights @ 1, 1: " << wg.weight(1, 1) << std::endl;

    std::cout << "Other info:\n";
    std::cout << "size: " << wg.size() << std::endl;
    std::cout << "is edge at 0, 1? " << wg.is_edge(0, 1) << std::endl;
    std::cout << "is edge at 1, 0? " << wg.is_edge(1, 0) << std::endl;

    std::cout << "\nExec depth first:\n";
    depth_first(graph::print_label<std::string>, wg, 0);

    std::cout << "\nExec breath first:\n";
    breadth_first(graph::print_label<std::string>, wg, 0);

    std::cout << "\n\nWeightedGraphSet:\n";

    graph::WeightedGraphSet<std::string, int> wg_set;

    wg_set.add_vertex("Rosemead");
    wg_set.add_vertex("Pasadena");
    wg_set.add_vertex("Alhambra");
    wg_set.add_vertex("El Monte");

    wg_set.add_edge(0, 1);
    wg_set.add_weight(0, 1, 100);
    wg_set.add_edge(0, 3);
    wg_set.add_edge(1, 1);
    wg_set.add_edge(1, 2);
    wg_set.add_edge(3, 1);
    wg_set.add_edge(3, 2);
    wg_set.add_edge(3, 3);

    std::cout << "Edges set:\n";
    wg_set.print_edges();

    std::cout << "Labels: ";
    wg_set.print_labels();
    std::cout << std::endl;

    std::cout << "Neighbors:\n";
    for(std::size_t i = 0; i < wg_set.size(); ++i) {
        std::cout << i << ": ";

        auto set = wg_set.neighbors(i);
        for(const auto& a : set) std::cout << wg_set[a] << ' ';

        std::cout << std::endl;
    }

    std::cout << "Weights @ 0, 1: " << wg_set.weight(0, 1) << std::endl;
    std::cout << "Weights @ 1, 1: " << wg_set.weight(1, 1) << std::endl;

    std::cout << "Other info:\n";
    std::cout << "size: " << wg_set.size() << std::endl;
    std::cout << "is edge at 0, 1? " << wg_set.is_edge(0, 1) << std::endl;
    std::cout << "is edge at 1, 0? " << wg_set.is_edge(1, 0) << std::endl;

    std::cout << "\nExec depth first:\n";
    depth_first(graph::print_label<std::string>, wg_set, 0);

    std::cout << "\nExec breath first:\n";
    breadth_first(graph::print_label<std::string>, wg_set, 0);

    return 0;
}