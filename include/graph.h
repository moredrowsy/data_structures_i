/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : graph
 * DESCRIPTION : This header defines tempalted GraphMatrix, WeightedGraphMatrix,
 *          GraphSet, and WeightedGraphSet. The weighted version of the graphs
 *          have the Dijkstra's sortest distance and path methods.
 *
 *          REQUIREMENTS: Weights in weighted graphs must be signed because
 *          negatives represent infinity during calculations.
 ******************************************************************************/
#ifndef GRAPH_H
#define GRAPH_H

#include <cassert>   // assert()
#include <iostream>  // stream

#include "bpt_map.h"  // Map class
#include "queue.h"    // Queue class
#include "set.h"      // Set class

namespace graph {

enum SIZE { CAPACITY = 2 };

template <typename T>
struct Edge {
    Edge(std::size_t s, std::size_t t, T w) : source(s), target(t), weight(w) {}
    std::size_t source;
    std::size_t target;
    T weight;
};

template <typename T>
class GraphMatrix {
public:
    // CONSTRUCTOR
    GraphMatrix(std::size_t capacity = CAPACITY);

    // BIG THREE
    virtual ~GraphMatrix();
    GraphMatrix(const GraphMatrix<T> &src);
    GraphMatrix<T> &operator=(const GraphMatrix<T> &rhs);

    // CAPACITY
    std::size_t capacity() const;
    std::size_t size() const;
    bool empty() const;

    // ELEMENT ACCESS
    const T &operator[](std::size_t vertex) const;
    T &operator[](std::size_t vertex);

    // MODIFIERS
    virtual void add_vertex(const T &label);
    bool add_edge(std::size_t source, std::size_t target);
    bool remove_edge(std::size_t source, std::size_t target);

    // OTHER
    set::Set<std::size_t> neighbors(std::size_t vertex);
    bool is_edge(std::size_t source, std::size_t target);

    void print_edges();
    void print_labels();
    void print_neighbors();

protected:
    std::size_t _capacity;  // allocation size
    std::size_t _vertices;  // total vertices
    bool **_edges;
    T *_labels;

    void allocate(bool **&edges, T *&labels, std::size_t &capacity);
    void deallocate(bool **&edges, T *&labels, std::size_t &capacity);
    void copy_internals(bool **src_edges, T *src_labels, std::size_t src_cap);
    void expand();
};

template <typename T, typename U>
class WeightedGraphMatrix : public GraphMatrix<T> {
public:
    // CONSTRUCTOR
    WeightedGraphMatrix(std::size_t capacity = CAPACITY);

    // BIG THREE
    ~WeightedGraphMatrix();
    WeightedGraphMatrix(const WeightedGraphMatrix<T, U> &src);
    WeightedGraphMatrix<T, U> &operator=(const WeightedGraphMatrix<T, U> &rhs);

    // ELEMENT ACCESS
    const U &weight(std::size_t source, std::size_t target) const;
    U &weight(std::size_t source, std::size_t target);

    // MODIFIERS
    void add_vertex(const T &label) override;
    bool add_weight(std::size_t source, std::size_t target, const U &weight);
    bool add_Edge(Edge<U> e);
    bool remove_Edge(Edge<U> e);

    // OTHER
    void print_weights();
    void calc_dijkstra_paths(std::size_t start);
    void print_dijkstra_paths();

private:
    U **_weights;                 // U must be signed
    std::vector<U> _distances;    // dijkstra distances
    std::vector<U> _predecessor;  // shortest path info

    void allocate(U **&weights, std::size_t capacity);
    void deallocate(U **&weights, std::size_t capacity);
    void copy_internals(U **src_weights, std::size_t src_cap);
    void expand(std::size_t old_capacity, std::size_t new_capacity);
};

template <typename T>
class GraphSet {
public:
    // CONSTRUCTOR
    GraphSet() {}

    // CAPACITY
    std::size_t size() const;
    bool empty() const;

    // ELEMENT ACCESS
    const T &operator[](std::size_t vertex) const;
    T &operator[](std::size_t vertex);

    // MODIFIERS
    virtual void add_vertex(const T &label);
    bool add_edge(std::size_t source, std::size_t target);
    bool remove_edge(std::size_t source, std::size_t target);

    // OTHER
    set::Set<std::size_t> neighbors(std::size_t vertex);
    bool is_edge(std::size_t source, std::size_t target);

    void print_edges();
    void print_labels();
    void print_neighbors();

protected:
    std::vector<set::Set<std::size_t>> _edges;
    std::vector<T> _labels;
};

template <typename T, typename U>
class WeightedGraphSet : public GraphSet<T> {
public:
    // ELEMENT ACCESS
    const U &weight(std::size_t source, std::size_t target) const;
    U &weight(std::size_t source, std::size_t target);

    // MODIFIERS
    void add_vertex(const T &label) override;
    bool add_weight(std::size_t source, std::size_t target, const U &weight);
    bool add_Edge(Edge<U> e);
    bool remove_Edge(Edge<U> e);

    // OTHER
    void print_weights();
    void calc_dijkstra_paths(std::size_t start);
    void print_dijkstra_paths();

private:
    std::vector<bpt_map::Map<std::size_t, U>> _weights;  // U must be signed
    std::vector<U> _distances;                           // dijkstra distances
    std::vector<U> _predecessor;                         // shortest path info
};

// recursive depth first search; need to be called using a wrapper
template <typename F, typename G, typename S>
void rec_dfs(F f, G &g, S v, set::Set<std::size_t> &visited);

// performs depth first that calls rec_dfs
template <typename F, typename G, typename S>
void depth_first(F f, G &g, S start);

// performs breadth first
template <typename F, typename G, typename S>
void breadth_first(F f, G &g, S start);

// checks for path from start to end in graph
template <typename G, typename S>
bool has_path(G &g, S start, S end);

// checks for path from start to end in graph
template <typename G, typename S>
void make_paths_set(G &g, S start, set::Set<std::size_t> &visited);

// calculate the shortest distances and paths (via predecessor)
template <typename G, typename S, typename U>
void dijkstra_shortest(G &g, S start, std::vector<U> &distances,
                       std::vector<U> &predecessor);

// find the min index for the distance array
template <typename U>
std::size_t find_min_distance_index(const std::vector<U> &distances,
                                    const set::Set<std::size_t> &unvisited);

// prints the dijkstra path with start and end vertex
template <typename U>
void print_paths(std::vector<U> &predecessor, std::size_t vertex);

// wrapper to print all shortest paths
template <typename U>
void print_all_paths(std::vector<U> &distances, std::vector<U> &predecessor);

// print label; use to test depth/breath first functions
template <typename T>
void print_label(T value);

// ----- GRAPHMATRIX IMPLEMENTATIONS -----

/*******************************************************************************
 * DESCRIPTION:
 *  Constructor with capacity with default.
 *
 * PRE-CONDITIONS:
 *  std::size_t capacity: capacity for internal arrays
 *
 * POST-CONDITIONS:
 *  new memory allocation
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
GraphMatrix<T>::GraphMatrix(std::size_t capacity)
    : _capacity(capacity), _vertices(0), _edges(nullptr), _labels(nullptr) {
    allocate(_edges, _labels, _capacity);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Destructor
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  memory deallocated
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
GraphMatrix<T>::~GraphMatrix() {
    deallocate(_edges, _labels, _capacity);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Copy CTOR
 *
 * PRE-CONDITIONS:
 *  const GraphMatrix<T> &src: source graph
 *
 * POST-CONDITIONS:
 *  new memory allocation
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
GraphMatrix<T>::GraphMatrix(const GraphMatrix<T> &src)
    : _capacity(src._capacity),
      _vertices(src._vertices),
      _edges(nullptr),
      _labels(nullptr) {
    allocate(_edges, _labels, _capacity);
    copy_internals(src._edges, src._labels, src._capacity);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Assignment OP.
 *
 * PRE-CONDITIONS:
 *  const GraphMatrix<T> &rhs: right hand side
 *
 * POST-CONDITIONS:
 *  new memory allocation
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
GraphMatrix<T> &GraphMatrix<T>::operator=(const GraphMatrix<T> &rhs) {
    if(this != &rhs) {
        deallocate(_edges, _labels, _capacity);

        _capacity = rhs._capacity;
        _vertices = rhs._vertices;

        allocate(_edges, _labels, _capacity);
        copy_internals(rhs._edges, rhs._labels, rhs._capacity);
    }
    return *this;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns capacity
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  std::size_t
 ******************************************************************************/
template <typename T>
std::size_t GraphMatrix<T>::capacity() const {
    return _capacity;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns size
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  std::size_t
 ******************************************************************************/
template <typename T>
std::size_t GraphMatrix<T>::size() const {
    return _vertices;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Checks if empty
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool GraphMatrix<T>::empty() const {
    return _vertices == 0;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Array operator access to label.
 *
 * PRE-CONDITIONS:
 *  std::size_t vertex: vertex < size()
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  const T &
 ******************************************************************************/
template <typename T>
const T &GraphMatrix<T>::operator[](std::size_t vertex) const {
    assert(vertex < size());
    return _labels[vertex];
}

/*******************************************************************************
 * DESCRIPTION:
 *  Array operator access to label.
 *
 * PRE-CONDITIONS:
 *  std::size_t vertex: vertex < size()
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  T &
 ******************************************************************************/
template <typename T>
T &GraphMatrix<T>::operator[](std::size_t vertex) {
    assert(vertex < size());
    return _labels[vertex];
}

/*******************************************************************************
 * DESCRIPTION:
 *  Add vertex with label.
 *
 * PRE-CONDITIONS:
 *  const T &label: label for vertex
 *
 * POST-CONDITIONS:
 *  size increase by 1 and aded to labels
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void GraphMatrix<T>::add_vertex(const T &label) {
    if(size() == _capacity) expand();

    // clear old values
    for(std::size_t i = 0; i <= _vertices; ++i) {
        _edges[i][_vertices] = false;
        _edges[_vertices][i] = false;
    }
    _labels[_vertices++] = label;  // add label and incrase _vertices count
}

/*******************************************************************************
 * DESCRIPTION:
 *  Connect source vertex to target vertex.
 *
 * PRE-CONDITIONS:
 *  std::size_t source: less than size()
 *  std::size_t target: less than size()
 *
 * POST-CONDITIONS:
 *  added to edges
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool GraphMatrix<T>::add_edge(std::size_t source, std::size_t target) {
    if(source < _vertices && target < _vertices) _edges[source][target] = true;

    return source < _vertices && target < _vertices;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Remove connection from source vertex to target vertex.
 *
 * PRE-CONDITIONS:
 *  std::size_t source: less than size()
 *  std::size_t target: less than size()
 *
 * POST-CONDITIONS:
 *  removed from edges
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool GraphMatrix<T>::remove_edge(std::size_t source, std::size_t target) {
    if(source < _vertices && target < _vertices) _edges[source][target] = false;

    return source < _vertices && target < _vertices;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns a set of neighbors at vertex.
 *
 * PRE-CONDITIONS:
 *  std::size_t vertex: less than size()
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  set::Set<std::size_t>
 ******************************************************************************/
template <typename T>
set::Set<std::size_t> GraphMatrix<T>::neighbors(std::size_t vertex) {
    set::Set<std::size_t> set;

    if(vertex < _vertices)
        for(std::size_t i = 0; i < _vertices; ++i)
            if(_edges[vertex][i]) set.insert(i);

    return set;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Checks if source and target have a connection.
 *
 * PRE-CONDITIONS:
 *  std::size_t source: less than size()
 *  std::size_t target: less than size()
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool GraphMatrix<T>::is_edge(std::size_t source, std::size_t target) {
    return source < _vertices && target < _vertices ? _edges[source][target]
                                                    : false;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Prints edges.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void GraphMatrix<T>::print_edges() {
    if(!empty())
        for(std::size_t i = 0; i < _vertices; ++i) {
            for(std::size_t j = 0; j < _vertices; ++j)
                std::cout << _edges[i][j] << ' ';
            std::cout << std::endl;
        }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Prints labels.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void GraphMatrix<T>::print_labels() {
    if(!empty())
        for(std::size_t i = 0; i < _vertices; ++i) {
            std::cout << _labels[i];
            if(i != size() - 1) std::cout << ", ";
        }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Prints neighbors.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void GraphMatrix<T>::print_neighbors() {
    if(!empty())
        for(std::size_t i = 0; i < _vertices; ++i) {
            std::cout << i << ": ";

            auto set = neighbors(i);
            for(auto it = set.begin(); it != set.end(); ++it) {
                std::cout << _labels[*it];
                if(*it != set.back()) std::cout << ", ";
            }
            std::cout << std::endl;
        }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Allocate memory for edges and labels.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  allocation
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void GraphMatrix<T>::allocate(bool **&edges, T *&labels,
                              std::size_t &capacity) {
    edges = new bool *[capacity];  // allocate 2d
    for(std::size_t i = 0; i < capacity; ++i) edges[i] = new bool[capacity]();

    labels = new T[capacity];  // allocate 1d
}

/*******************************************************************************
 * DESCRIPTION:
 *  Deallocate memory for edges and labels.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  deallocation
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void GraphMatrix<T>::deallocate(bool **&edges, T *&labels,
                                std::size_t &capacity) {
    for(std::size_t i = 0; i < capacity; ++i) delete[] edges[i];

    delete[] edges;
    delete[] labels;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Copy unique values for edges and labels.
 *
 * PRE-CONDITIONS:
 *  bool **src_edges   : source edges
 *  T *src_labels      : source labels
 *  std::size_t src_cap: source capacity
 *
 * POST-CONDITIONS:
 *  copied to new values
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void GraphMatrix<T>::copy_internals(bool **src_edges, T *src_labels,
                                    std::size_t src_cap) {
    for(std::size_t i = 0; i < src_cap; ++i) {
        _labels[i] = src_labels[i];

        for(std::size_t j = 0; j < src_cap; ++j) _edges[i][j] = src_edges[i][j];
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Expand dynamic array with 2 times current capacity
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  capacity 2x
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void GraphMatrix<T>::expand() {
    // store old states
    std::size_t old_cap = _capacity;
    bool **old_edges = _edges;
    T *old_labels = _labels;

    // create newly expanded states
    _capacity = 2 * _capacity;
    allocate(_edges, _labels, _capacity);
    copy_internals(old_edges, old_labels, old_cap);  // copy old states

    deallocate(old_edges, old_labels, old_cap);  // delete old states
}

// ----- WEIGHTEDGRAPHMATRIX IMPLEMENTATIONS -----

/*******************************************************************************
 * DESCRIPTION:
 *  Constructor with capacity with default.
 *
 * PRE-CONDITIONS:
 *  std::size_t capacity: capacity for internal arrays
 *
 * POST-CONDITIONS:
 *  new memory allocation
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename U>
WeightedGraphMatrix<T, U>::WeightedGraphMatrix(std::size_t capacity)
    : GraphMatrix<T>(capacity), _weights(nullptr) {
    allocate(_weights, GraphMatrix<T>::capacity());
}

/*******************************************************************************
 * DESCRIPTION:
 *  Destructor
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  memory deallocated
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename U>
WeightedGraphMatrix<T, U>::~WeightedGraphMatrix() {
    deallocate(_weights, GraphMatrix<T>::capacity());
}

/*******************************************************************************
 * DESCRIPTION:
 *  Copy CTOR
 *
 * PRE-CONDITIONS:
 *  const GraphMatrix<T> &src: source graph
 *
 * POST-CONDITIONS:
 *  new memory allocation
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename U>
WeightedGraphMatrix<T, U>::WeightedGraphMatrix(
    const WeightedGraphMatrix<T, U> &src)
    : GraphMatrix<T>(src) {
    allocate(_weights, src.capacity());
    copy_internals(src._weights, src.capacity());
}

/*******************************************************************************
 * DESCRIPTION:
 *  Assignment OP.
 *
 * PRE-CONDITIONS:
 *  const GraphMatrix<T> &rhs: right hand side
 *
 * POST-CONDITIONS:
 *  new memory allocation
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename U>
WeightedGraphMatrix<T, U> &WeightedGraphMatrix<T, U>::operator=(
    const WeightedGraphMatrix<T, U> &rhs) {
    if(this != &rhs) {
        GraphMatrix<T>::operator=(rhs);

        deallocate(_weights, GraphMatrix<T>::capacity());
        allocate(_weights, rhs.capacity());
        copy_internals(rhs._weights, rhs.capacity());
    }
    return *this;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Return weight for edge from source to target.
 *
 * PRE-CONDITIONS:
 *  std::size_t source: less than size()
 *  std::size_t target: less than size()
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  const U &
 ******************************************************************************/
template <typename T, typename U>
const U &WeightedGraphMatrix<T, U>::weight(std::size_t source,
                                           std::size_t target) const {
    assert(source < GraphMatrix<T>::size());
    assert(target < GraphMatrix<T>::size());

    return _weights[source][target];
}

/*******************************************************************************
 * DESCRIPTION:
 *  Return weight for edge from source to target.
 *
 * PRE-CONDITIONS:
 *  std::size_t source: less than size()
 *  std::size_t target: less than size()
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  U &
 ******************************************************************************/
template <typename T, typename U>
U &WeightedGraphMatrix<T, U>::weight(std::size_t source, std::size_t target) {
    assert(source < GraphMatrix<T>::size());
    assert(target < GraphMatrix<T>::size());

    return _weights[source][target];
}

/*******************************************************************************
 * DESCRIPTION:
 *  Add vertex with label and expand if necessary
 *
 * PRE-CONDITIONS:
 *  const T &label: label for vertex
 *
 * POST-CONDITIONS:
 *  size increase by 1 and aded to labels
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename U>
void WeightedGraphMatrix<T, U>::add_vertex(const T &label) {
    std::size_t old_capacity = GraphMatrix<T>::capacity();
    GraphMatrix<T>::add_vertex(label);

    if(GraphMatrix<T>::capacity() > old_capacity)
        expand(old_capacity, GraphMatrix<T>::capacity());
}

/*******************************************************************************
 * DESCRIPTION:
 *  Add weight from source vertex to target vertex.
 *
 * PRE-CONDITIONS:
 *  std::size_t source: less than size()
 *  std::size_t target: less than size()
 *  const U &weight   : weight value
 *
 * POST-CONDITIONS:
 *  added to weights
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T, typename U>
bool WeightedGraphMatrix<T, U>::add_weight(std::size_t source,
                                           std::size_t target,
                                           const U &weight) {
    if(source < GraphMatrix<T>::size() && target < GraphMatrix<T>::size())
        _weights[source][target] = weight;
    return source < GraphMatrix<T>::size() && target < GraphMatrix<T>::size();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Add weight from source vertex to target vertex.
 *
 * PRE-CONDITIONS:
 *  Edge<U> e: Edge with source, target vertices and weight of type U
 *
 * POST-CONDITIONS:
 *  added an Edge from source to target with weight
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T, typename U>
bool WeightedGraphMatrix<T, U>::add_Edge(Edge<U> e) {
    if(this->add_edge(e.source, e.target)) {
        _weights[e.source][e.target] = e.weight;
        return true;
    } else
        return false;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Add weight from source vertex to target vertex.
 *
 * PRE-CONDITIONS:
 *  Edge<U> e: Edge with source, target vertices and weight of type U
 *
 * POST-CONDITIONS:
 *  removed an Edge from source to target with weight
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T, typename U>
bool WeightedGraphMatrix<T, U>::remove_Edge(Edge<U> e) {
    if(this->add_remove(e.source, e.target)) {
        _weights[e.source][e.target] = 0;
        return true;
    } else
        return false;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Prints weights.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename U>
void WeightedGraphMatrix<T, U>::print_weights() {
    if(!GraphMatrix<T>::empty())
        for(std::size_t i = 0; i < GraphMatrix<T>::size(); ++i) {
            for(std::size_t j = 0; j < GraphMatrix<T>::size(); ++j)
                std::cout << _weights[i][j] << ' ';
            std::cout << std::endl;
        }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Calculate short distances and paths using Dijkstra's algorithm.
 *
 * PRE-CONDITIONS:
 *  std::size_t start          : start vertext
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename U>
void WeightedGraphMatrix<T, U>::calc_dijkstra_paths(std::size_t start) {
    _distances.clear();
    _predecessor.clear();
    dijkstra_shortest(*this, start, _distances, _predecessor);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Prints distances and paths array along with the short paths.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename U>
void WeightedGraphMatrix<T, U>::print_dijkstra_paths() {
    std::cout << "Distances: " << _distances << std::endl;
    std::cout << "Predecessor: " << _predecessor << std::endl;

    std::cout << "Paths:" << std::endl;
    print_all_paths(_distances, _predecessor);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Allocate memory for weights.
 *
 * PRE-CONDITIONS:
 *  U **&weights        : weights array
 *  std::size_t capacity: capacity for weights
 *
 * POST-CONDITIONS:
 *  allocation
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename U>
void WeightedGraphMatrix<T, U>::allocate(U **&weights, std::size_t capacity) {
    weights = new U *[capacity];
    for(std::size_t i = 0; i < capacity; ++i) weights[i] = new U[capacity]();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Deallocate memory for weights.
 *
 * PRE-CONDITIONS:
 *  U **&weights        : weights array
 *  std::size_t capacity: capacity for weights
 *
 * POST-CONDITIONS:
 *  deallocation
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename U>
void WeightedGraphMatrix<T, U>::deallocate(U **&weights, std::size_t capacity) {
    for(std::size_t i = 0; i < capacity; ++i) delete[] weights[i];

    delete[] weights;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Copy unique values for edges and labels.
 *
 * PRE-CONDITIONS:
 *  U **src_weights    : source weights
 *  std::size_t src_cap: source capacity
 *
 * POST-CONDITIONS:
 *  copied to new values
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename U>
void WeightedGraphMatrix<T, U>::copy_internals(U **src_weights,
                                               std::size_t src_cap) {
    for(std::size_t i = 0; i < src_cap; ++i)
        for(std::size_t j = 0; j < src_cap; ++j)
            _weights[i][j] = src_weights[i][j];
}

/*******************************************************************************
 * DESCRIPTION:
 *  Expand dynamic array with 2 times current capacity
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  capacity 2x
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename U>
void WeightedGraphMatrix<T, U>::expand(std::size_t old_capacity,
                                       std::size_t new_capacity) {
    U **old_weights = _weights;

    allocate(_weights, new_capacity);
    copy_internals(old_weights, old_capacity);
    deallocate(old_weights, old_capacity);
}

// ----- GRAPHSET IMPLEMENTATIONS -----

/*******************************************************************************
 * DESCRIPTION:
 *  Returns size
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  std::size_t
 ******************************************************************************/
template <typename T>
std::size_t GraphSet<T>::size() const {
    return _edges.size();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Checks if empty
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool GraphSet<T>::empty() const {
    return _edges.empty();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Array operator access to label.
 *
 * PRE-CONDITIONS:
 *  std::size_t vertex: vertex < size()
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  const T &
 ******************************************************************************/
template <typename T>
const T &GraphSet<T>::operator[](std::size_t vertex) const {
    assert(vertex < _edges.size());
    return _labels[vertex];
}

/*******************************************************************************
 * DESCRIPTION:
 *  Array operator access to label.
 *
 * PRE-CONDITIONS:
 *  std::size_t vertex: vertex < size()
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  T &
 ******************************************************************************/
template <typename T>
T &GraphSet<T>::operator[](std::size_t vertex) {
    assert(vertex < _edges.size());
    return _labels[vertex];
}

/*******************************************************************************
 * DESCRIPTION:
 *  Add vertex with label.
 *
 * PRE-CONDITIONS:
 *  const T &label: label for vertex
 *
 * POST-CONDITIONS:
 *  size increase by 1 and aded to labels
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void GraphSet<T>::add_vertex(const T &label) {
    _edges.emplace_back();
    _labels.emplace_back(label);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Connect source vertex to target vertex.
 *
 * PRE-CONDITIONS:
 *  std::size_t source: less than size()
 *  std::size_t target: less than size()
 *
 * POST-CONDITIONS:
 *  added to edges
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool GraphSet<T>::add_edge(std::size_t source, std::size_t target) {
    bool is_inserted = false;

    if(source < _edges.size() && target < _edges.size())
        is_inserted = _edges[source].insert(target);

    return is_inserted;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Remove connection from source vertex to target vertex.
 *
 * PRE-CONDITIONS:
 *  std::size_t source: less than size()
 *  std::size_t target: less than size()
 *
 * POST-CONDITIONS:
 *  removed from edges
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool GraphSet<T>::remove_edge(std::size_t source, std::size_t target) {
    bool is_removed = false;

    if(source < _edges.size() && target < _edges.size() &&
       !_edges[source].empty())
        is_removed = _edges[source].erase(target);

    return is_removed;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns a set of neighbors at vertex.
 *
 * PRE-CONDITIONS:
 *  std::size_t vertex: less than size()
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  set::Set<std::size_t>
 ******************************************************************************/
template <typename T>
set::Set<std::size_t> GraphSet<T>::neighbors(std::size_t vertex) {
    return vertex < _edges.size() ? _edges[vertex] : set::Set<std::size_t>();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Checks if source and target have a connection.
 *
 * PRE-CONDITIONS:
 *  std::size_t source: less than size()
 *  std::size_t target: less than size()
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool GraphSet<T>::is_edge(std::size_t source, std::size_t target) {
    bool is_edge = false;

    if(source < _edges.size() && target < _edges.size())
        is_edge = _edges[source].contains(target);

    return is_edge;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Prints edges.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void GraphSet<T>::print_edges() {
    if(!empty())
        for(std::size_t i = 0; i < _edges.size(); ++i) {
            for(auto const &a : _edges[i]) std::cout << a << ' ';
            std::cout << std::endl;
        }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Prints labels.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void GraphSet<T>::print_neighbors() {
    if(!empty())
        for(std::size_t i = 0; i < size(); ++i) {
            std::cout << i << ": ";

            auto set = neighbors(i);
            for(auto it = set.begin(); it != set.end(); ++it) {
                std::cout << _labels[*it];
                if(*it != set.back()) std::cout << ", ";
            }
            std::cout << std::endl;
        }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Prints neighbors.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void GraphSet<T>::print_labels() {
    if(!empty())
        for(std::size_t i = 0; i < _labels.size(); ++i) {
            std::cout << _labels[i];
            if(i != size() - 1) std::cout << ", ";
        }
}

// ----- WEIGHTEDGRAPHSET IMPLEMENTATIONS -----

/*******************************************************************************
 * DESCRIPTION:
 *  Return weight for edge from source to target.
 *
 * PRE-CONDITIONS:
 *  std::size_t source: less than size()
 *  std::size_t target: less than size()
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  const U &
 ******************************************************************************/
template <typename T, typename U>
const U &WeightedGraphSet<T, U>::weight(std::size_t source,
                                        std::size_t target) const {
    assert(source < GraphSet<T>::size());
    assert(target < GraphSet<T>::size());

    return _weights[source][target];
}

/*******************************************************************************
 * DESCRIPTION:
 *  Return weight for edge from source to target.
 *
 * PRE-CONDITIONS:
 *  std::size_t source: less than size()
 *  std::size_t target: less than size()
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  U &
 ******************************************************************************/
template <typename T, typename U>
U &WeightedGraphSet<T, U>::weight(std::size_t source, std::size_t target) {
    assert(source < GraphSet<T>::size());
    assert(target < GraphSet<T>::size());

    return _weights[source][target];
}

/*******************************************************************************
 * DESCRIPTION:
 *  Add vertex with label and expand if necessary
 *
 * PRE-CONDITIONS:
 *  const T &label: label for vertex
 *
 * POST-CONDITIONS:
 *  size increase by 1 and aded to labels
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename U>
void WeightedGraphSet<T, U>::add_vertex(const T &label) {
    GraphSet<T>::add_vertex(label);
    _weights.emplace_back();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Add weight from source vertex to target vertex.
 *
 * PRE-CONDITIONS:
 *  std::size_t source: less than size()
 *  std::size_t target: less than size()
 *  const U &weight   : weight value
 *
 * POST-CONDITIONS:
 *  added to weights
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T, typename U>
bool WeightedGraphSet<T, U>::add_weight(std::size_t source, std::size_t target,
                                        const U &weight) {
    if(source < GraphSet<T>::size() && target < GraphSet<T>::size())
        _weights[source][target] = weight;

    return source < GraphSet<T>::size() && target < GraphSet<T>::size();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Add weight from source vertex to target vertex.
 *
 * PRE-CONDITIONS:
 *  Edge<U> e: Edge with source, target vertices and weight of type U
 *
 * POST-CONDITIONS:
 *  added an Edge from source to target with weight
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T, typename U>
bool WeightedGraphSet<T, U>::add_Edge(Edge<U> e) {
    if(this->add_edge(e.source, e.target)) {
        _weights[e.source][e.target] = e.weight;
        return true;
    } else
        return false;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Add weight from source vertex to target vertex.
 *
 * PRE-CONDITIONS:
 *  Edge<U> e: Edge with source, target vertices and weight of type U
 *
 * POST-CONDITIONS:
 *  removed an Edge from source to target with weight
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T, typename U>
bool WeightedGraphSet<T, U>::remove_Edge(Edge<U> e) {
    if(this->add_remove(e.source, e.target)) {
        _weights[e.source][e.target] = 0;
        return true;
    } else
        return false;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Prints weights.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename U>
void WeightedGraphSet<T, U>::print_weights() {
    if(!GraphSet<T>::empty())
        for(std::size_t i = 0; i < GraphSet<T>::size(); ++i) {
            for(std::size_t j = 0; j < GraphSet<T>::size(); ++j)
                std::cout << _weights[i][j] << ' ';
            std::cout << std::endl;
        }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Calculate short distances and paths using Dijkstra's algorithm.
 *
 * PRE-CONDITIONS:
 *  std::size_t start          : start vertext
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename U>
void WeightedGraphSet<T, U>::calc_dijkstra_paths(std::size_t start) {
    _distances.clear();
    _predecessor.clear();
    dijkstra_shortest(*this, start, _distances, _predecessor);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Prints distances and paths array along with the short paths.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename U>
void WeightedGraphSet<T, U>::print_dijkstra_paths() {
    std::cout << "Distances: " << _distances << std::endl;
    std::cout << "Predecessor: " << _predecessor << std::endl;

    std::cout << "Paths:" << std::endl;
    print_all_paths(_distances, _predecessor);
}

// ----- TRAVERSAL IMPLEMENTATIONS -----

/*******************************************************************************
 * DESCRIPTION:
 *  Recursively perform depth first search.
 *
 * PRE-CONDITIONS:
 *  F f         : function to process label
 *  G &g        : graph object
 *  S v         : vertex of size type
 *  bool *marked: array for marked vertices
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename F, typename G, typename S>
void rec_dfs(F f, G &g, S v, set::Set<std::size_t> &visited) {
    set::Set<std::size_t> neighbors = g.neighbors(v);

    visited.insert(v);  // mark vertex v as visited
    f(g[v]);            // process g[v]

    for(const auto &vertex : neighbors)      // loop neighbors set
        if(!visited.contains(vertex))        // if not visited
            rec_dfs(f, g, vertex, visited);  // then recurse
}

/*******************************************************************************
 * DESCRIPTION:
 *  Wrapper to call rec_dfs for depth first search.
 *
 * PRE-CONDITIONS:
 *  F f         : function to process label
 *  G &g        : graph object
 *  S start     : start vertex of size type
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename F, typename G, typename S>
void depth_first(F f, G &g, S start) {
    set::Set<std::size_t> visited;  // empty visited set
    assert(start < (S)g.size());
    rec_dfs(f, g, start, visited);  // call rec_defs to process graph
}

/*******************************************************************************
 * DESCRIPTION:
 *  Perform breath first search.
 *
 * PRE-CONDITIONS:
 *  F f         : function to process label
 *  G &g        : graph object
 *  S start     : start vertex of size type
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename F, typename G, typename S>
void breadth_first(F f, G &g, S start) {
    assert(start < (S)g.size());

    set::Set<std::size_t> visited;           // visited set
    set::Set<std::size_t> neighbors;         // neighbors set
    queue::Queue<std::size_t> vertex_queue;  // traversal queue

    visited.insert(start);     // add start to visited
    f(g[start]);               // process label
    vertex_queue.push(start);  // push vertex to queue for traversal

    do {  // pop queue to get vertex and get neighbor's set of vertex
        neighbors = g.neighbors(vertex_queue.pop());

        // mark and process the unvisited neighbors; place them in the queue
        for(const auto &a : neighbors) {  // loop neighbors set
            if(!visited.contains(a)) {    // if not visited
                visited.insert(a);        // add to visited
                f(g[a]);                  // process label
                vertex_queue.push(a);     // then push vertex to queue
            }
        }
    } while(!vertex_queue.empty());
}

/*******************************************************************************
 * DESCRIPTION:
 *  Checks if a path exist from star to end
 *
 * PRE-CONDITIONS:
 *  G &g        : graph object
 *  S start     : start vertex of size type
 *  S end       : end vertex of size type
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename G, typename S>
bool has_path(G &g, S start, S end) {
    assert(start < (S)g.size());

    bool is_found = false;
    set::Set<std::size_t> visited;           // visited set
    set::Set<std::size_t> neighbors;         // neighbors set
    queue::Queue<std::size_t> vertex_queue;  // traversal queue

    visited.insert(start);     // add start to visited
    vertex_queue.push(start);  // push vertex for traversal

    do {  // pop queue to get vertex and get neighbor's set of vertices
        neighbors = g.neighbors(vertex_queue.pop());

        if(visited.contains(end) || neighbors.contains(end)) {
            is_found = true;
            break;
        }

        // mark and process the unvisited neighbors; place them in the queue
        for(const auto &a : neighbors) {  // loop neighbors set
            if(!visited.contains(a)) {    // if not visited
                visited.insert(a);        // add to visited
                vertex_queue.push(a);     // then push vertex to queue
            }
        }
    } while(!vertex_queue.empty());

    return is_found;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Make a set of visited vertices from start of graph. If vertex has been
 *  visited, then path from start to vertex exists.
 *
 * PRE-CONDITIONS:
 *  G &g                        : graph object
 *  S start                     : start of graph of size type
 *  set::Set<std::size_t> &paths: empty visited set paths
 *
 * POST-CONDITIONS:
 *  set::Set<std::size_t> &paths: populated with visited vertices
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename G, typename S>
void make_paths_set(G &g, S start, set::Set<std::size_t> &visited) {
    assert(start < (S)g.size());

    visited.clear();                         // visited paths set
    set::Set<std::size_t> neighbors;         // neighbors set
    queue::Queue<std::size_t> vertex_queue;  // traversal queue

    visited.insert(start);     // add start to visited
    vertex_queue.push(start);  // push vertex for traversal

    do {  // pop queue to get vertex and get neighbor's set of vertices
        neighbors = g.neighbors(vertex_queue.pop());

        // mark and process the unvisited neighbors; place them in the queue
        for(const auto &a : neighbors) {  // loop neighbors set
            if(!visited.contains(a)) {    // if not visited
                visited.insert(a);        // add to visited
                vertex_queue.push(a);     // then push vertex to queue
            }
        }
    } while(!vertex_queue.empty());
}

/*******************************************************************************
 * DESCRIPTION:
 *  Finds the short distances and paths via distances and predecessor vector.
 *
 * PRE-CONDITIONS:
 *  G &g                       : graph object
 *  std::vector<U> &distances  : distances array
 *  std::vector<U> &predecessor: short path info array
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename G, typename S, typename U>
void dijkstra_shortest(G &g, S start, std::vector<U> &distances,
                       std::vector<U> &predecessor) {
    assert(start < (S)g.size());

    set::Set<std::size_t> unvisited;  // keep track of unvisited vertices
    set::Set<std::size_t> paths;      // set of 'start has path to v'
    std::size_t size = g.size();      // size of vertices
    std::size_t next;                 // next vertex to look at
    U sum;                            // sum of the weighted distances

    // init distance and predecessor to -1 and fill unvisited set
    for(std::size_t i = 0; i < size; ++i) {
        distances.emplace_back(-1);
        predecessor.emplace_back(-1);
        unvisited.insert(i);
    }
    make_paths_set(g, start, paths);  // get set of paths from start to v
    distances[start] = 0;             // init start to itself as 0 distance

    for(std::size_t i = 1; i < size; ++i) {
        // find the unvisited index with minimum weights in distances
        next = find_min_distance_index(distances, unvisited);
        unvisited.erase(next);  // remove visited

        if(paths.contains(next))                   // if next exists in paths
            for(const auto &vertex : unvisited) {  // then loop unvisited
                if(g.is_edge(next, vertex)) {      // if edge exist
                    sum = distances[next] + g.weight(next, vertex);

                    // if new sum is smaller or dist @ v is negative
                    if(sum < distances[vertex] || distances[vertex] < 0) {
                        distances[vertex] = sum;     // update shortest dist.
                        predecessor[vertex] = next;  // uppdate shortest path
                    }
                }
            }
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Finds the unvisited index that have the minimum positive number.
 *
 * PRE-CONDITIONS:
 *  const std::vector<U> &distances       : distances array
 *  std::size_t size                      : size of array
 *  const set::Set<std::size_t> &unvisited: unvisited sets
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  std::size_t
 ******************************************************************************/
template <typename U>
std::size_t find_min_distance_index(const std::vector<U> &distances,
                                    const set::Set<std::size_t> &unvisited) {
    std::size_t size = distances.size();
    std::size_t min_v = unvisited.empty() ? 0 : *unvisited.begin();
    assert(min_v < size);

    for(const auto &vertex : unvisited) {
        assert(vertex < size);

        if(distances[vertex] >= 0) {
            if(distances[min_v] < 0) min_v = vertex;
            if(distances[vertex] < distances[min_v]) min_v = vertex;
        }
    }
    return min_v;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Prints the predecessor paths @ start to vertex.
 *
 * PRE-CONDITIONS:
 *  std::vector<U> &predecessor: shortest paths info array
 *  std::size_t vertex         : vertex to go in predecessor
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  std::size_t
 ******************************************************************************/
template <typename U>
void print_paths(std::vector<U> &predecessor, std::size_t vertex) {
    if(predecessor[vertex] >= 0) {                      // if path exists
        print_paths(predecessor, predecessor[vertex]);  // then recurse
        std::cout << " -> " << vertex;                  // print when coming out
    } else                                              // if not exist
        std::cout << vertex;                            // then just print
}

/*******************************************************************************
 * DESCRIPTION:
 *  Prints all the paths from distances with predecessor info array.
 *
 * PRE-CONDITIONS:
 *  std::vector<U> &distances  : distances array
 *  std::vector<U> &predecessor: shortest path info array
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  std::size_t
 ******************************************************************************/
template <typename U>
void print_all_paths(std::vector<U> &distances, std::vector<U> &predecessor) {
    for(std::size_t i = 0; i < distances.size(); ++i) {
        std::cout << "To " << i << ": ";                    // if there is dist
        if(distances[i] >= 0) print_paths(predecessor, i);  // then print
        std::cout << std::endl;
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Prints value.
 *
 * PRE-CONDITIONS:
 *  T value: value to process
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void print_label(T value) {
    std::cout << value << std::endl;
}

}  // namespace graph

#endif  // GRAPH_H
