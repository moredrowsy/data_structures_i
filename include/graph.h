#ifndef GRAPH_H
#define GRAPH_H

#include <cassert>    // assert()
#include <iostream>   // stream
#include "bpt_map.h"  // Map class
#include "queue.h"    // Queue class
#include "set.h"      // Set class

namespace graph {

enum SIZE { CAPACITY = 2 };

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

private:
    U **_weights;

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

private:
    std::vector<bpt_map::Map<std::size_t, U>> _weights;
};

template <typename F, typename G, typename S>
void rec_dfs(F f, G &g, S v, bool *marked);

template <typename F, typename G, typename S>
void depth_first(F f, G &g, S start);

template <typename F, typename G, typename S>
void breadth_first(F f, G &g, S start);

template <typename T>
void print_label(T value);

// ----- GRAPHMATRIX IMPLEMENTATIONS -----

template <typename T>
GraphMatrix<T>::GraphMatrix(std::size_t capacity)
    : _capacity(capacity), _vertices(0), _edges(nullptr), _labels(nullptr) {
    allocate(_edges, _labels, _capacity);
}

template <typename T>
GraphMatrix<T>::~GraphMatrix() {
    deallocate(_edges, _labels, _capacity);
}

template <typename T>
GraphMatrix<T>::GraphMatrix(const GraphMatrix<T> &src)
    : _capacity(src._capacity),
      _vertices(src._vertices),
      _edges(nullptr),
      _labels(nullptr) {
    allocate(_edges, _labels, _capacity);
    copy_internals(src._edges, src._labels, src._capacity);
}

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

template <typename T>
std::size_t GraphMatrix<T>::capacity() const {
    return _capacity;
}

template <typename T>
std::size_t GraphMatrix<T>::size() const {
    return _vertices;
}

template <typename T>
bool GraphMatrix<T>::empty() const {
    return _vertices == 0;
}

template <typename T>
const T &GraphMatrix<T>::operator[](std::size_t vertex) const {
    assert(vertex < size());
    return _labels[vertex];
}

template <typename T>
T &GraphMatrix<T>::operator[](std::size_t vertex) {
    assert(vertex < size());
    return _labels[vertex];
}

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

template <typename T>
bool GraphMatrix<T>::add_edge(std::size_t source, std::size_t target) {
    if(source < _vertices && target < _vertices) _edges[source][target] = true;

    return source < _vertices && target < _vertices;
}

template <typename T>
bool GraphMatrix<T>::remove_edge(std::size_t source, std::size_t target) {
    if(source < _vertices && target < _vertices) _edges[source][target] = false;

    return source < _vertices && target < _vertices;
}

template <typename T>
set::Set<std::size_t> GraphMatrix<T>::neighbors(std::size_t vertex) {
    set::Set<std::size_t> set;

    if(vertex < _vertices)
        for(std::size_t i = 0; i < _vertices; ++i)
            if(_edges[vertex][i]) set.insert(i);

    return set;
}

template <typename T>
bool GraphMatrix<T>::is_edge(std::size_t source, std::size_t target) {
    return source < _vertices && target < _vertices ? _edges[source][target]
                                                    : false;
}

template <typename T>
void GraphMatrix<T>::print_edges() {
    if(!empty())
        for(std::size_t i = 0; i < _vertices; ++i) {
            for(std::size_t j = 0; j < _vertices; ++j)
                std::cout << _edges[i][j] << ' ';
            std::cout << std::endl;
        }
}

template <typename T>
void GraphMatrix<T>::print_labels() {
    if(!empty())
        for(std::size_t i = 0; i < _vertices; ++i)
            std::cout << _labels[i] << ' ';
}

template <typename T>
void GraphMatrix<T>::allocate(bool **&edges, T *&labels,
                              std::size_t &capacity) {
    edges = new bool *[capacity];  // allocate 2d
    for(std::size_t i = 0; i < capacity; ++i) edges[i] = new bool[capacity]();

    labels = new T[capacity];  // allocate 1d
}

template <typename T>
void GraphMatrix<T>::deallocate(bool **&edges, T *&labels,
                                std::size_t &capacity) {
    for(std::size_t i = 0; i < capacity; ++i) delete[] edges[i];

    delete[] edges;
    delete[] labels;
}

template <typename T>
void GraphMatrix<T>::copy_internals(bool **src_edges, T *src_labels,
                                    std::size_t src_cap) {
    for(std::size_t i = 0; i < src_cap; ++i) {
        _labels[i] = src_labels[i];

        for(std::size_t j = 0; j < src_cap; ++j) _edges[i][j] = src_edges[i][j];
    }
}

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
template <typename T, typename U>
WeightedGraphMatrix<T, U>::WeightedGraphMatrix(std::size_t capacity)
    : GraphMatrix<T>(capacity), _weights(nullptr) {
    allocate(_weights, GraphMatrix<T>::capacity());
}

template <typename T, typename U>
WeightedGraphMatrix<T, U>::~WeightedGraphMatrix() {
    deallocate(_weights, GraphMatrix<T>::capacity());
}

template <typename T, typename U>
WeightedGraphMatrix<T, U>::WeightedGraphMatrix(
    const WeightedGraphMatrix<T, U> &src)
    : GraphMatrix<T>(src) {
    allocate(_weights, src.capacity());
    copy_internals(src._weights, src.capacity());
}

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

template <typename T, typename U>
const U &WeightedGraphMatrix<T, U>::weight(std::size_t source,
                                           std::size_t target) const {
    assert(source < GraphMatrix<T>::size());
    assert(target < GraphMatrix<T>::size());

    return _weights[source][target];
}

template <typename T, typename U>
U &WeightedGraphMatrix<T, U>::weight(std::size_t source, std::size_t target) {
    assert(source < GraphMatrix<T>::size());
    assert(target < GraphMatrix<T>::size());

    return _weights[source][target];
}

template <typename T, typename U>
void WeightedGraphMatrix<T, U>::add_vertex(const T &label) {
    std::size_t old_capacity = GraphMatrix<T>::capacity();
    GraphMatrix<T>::add_vertex(label);

    if(GraphMatrix<T>::capacity() > old_capacity)
        expand(old_capacity, GraphMatrix<T>::capacity());
}

template <typename T, typename U>
bool WeightedGraphMatrix<T, U>::add_weight(std::size_t source,
                                           std::size_t target,
                                           const U &weight) {
    if(source < GraphMatrix<T>::size() && target < GraphMatrix<T>::size())
        _weights[source][target] = weight;
    return source < GraphMatrix<T>::size() && target < GraphMatrix<T>::size();
}

template <typename T, typename U>
void WeightedGraphMatrix<T, U>::allocate(U **&weights, std::size_t capacity) {
    weights = new U *[capacity];
    for(std::size_t i = 0; i < capacity; ++i) weights[i] = new U[capacity]();
}

template <typename T, typename U>
void WeightedGraphMatrix<T, U>::deallocate(U **&weights, std::size_t capacity) {
    for(std::size_t i = 0; i < capacity; ++i) delete[] weights[i];

    delete[] weights;
}

template <typename T, typename U>
void WeightedGraphMatrix<T, U>::copy_internals(U **src_weights,
                                               std::size_t src_cap) {
    for(std::size_t i = 0; i < src_cap; ++i)
        for(std::size_t j = 0; j < src_cap; ++j)
            _weights[i][j] = src_weights[i][j];
}

template <typename T, typename U>
void WeightedGraphMatrix<T, U>::expand(std::size_t old_capacity,
                                       std::size_t new_capacity) {
    U **old_weights = _weights;

    allocate(_weights, new_capacity);
    copy_internals(old_weights, old_capacity);
    deallocate(old_weights, old_capacity);
}

// ----- GRAPHSET IMPLEMENTATIONS -----

template <typename T>
std::size_t GraphSet<T>::size() const {
    return _edges.size();
}

template <typename T>
bool GraphSet<T>::empty() const {
    return _edges.empty();
}

template <typename T>
const T &GraphSet<T>::operator[](std::size_t vertex) const {
    assert(vertex < _edges.size());
    return _labels[vertex];
}

template <typename T>
T &GraphSet<T>::operator[](std::size_t vertex) {
    assert(vertex < _edges.size());
    return _labels[vertex];
}

template <typename T>
void GraphSet<T>::add_vertex(const T &label) {
    _edges.emplace_back();
    _labels.emplace_back(label);
}

template <typename T>
bool GraphSet<T>::add_edge(std::size_t source, std::size_t target) {
    bool is_inserted = false;

    if(source < _edges.size() && target < _edges.size())
        is_inserted = _edges[source].insert(target);

    return is_inserted;
}

template <typename T>
bool GraphSet<T>::remove_edge(std::size_t source, std::size_t target) {
    bool is_removed = false;

    if(source < _edges.size() && target < _edges.size() &&
       !_edges[source].empty())
        is_removed = _edges[source].erase(target);

    return is_removed;
}

template <typename T>
set::Set<std::size_t> GraphSet<T>::neighbors(std::size_t vertex) {
    return vertex < _edges.size() ? _edges[vertex] : set::Set<std::size_t>();
}

template <typename T>
bool GraphSet<T>::is_edge(std::size_t source, std::size_t target) {
    bool is_edge = false;

    if(source < _edges.size() && target < _edges.size())
        is_edge = _edges[source].contains(target);

    return is_edge;
}

template <typename T>
void GraphSet<T>::print_edges() {
    if(!empty())
        for(std::size_t i = 0; i < _edges.size(); ++i) {
            for(auto const &a : _edges[i]) std::cout << a << ' ';
            std::cout << std::endl;
        }
}

template <typename T>
void GraphSet<T>::print_labels() {
    if(!empty())
        for(std::size_t i = 0; i < _labels.size(); ++i)
            std::cout << _labels[i] << ' ' << std::endl;
}

// ----- WEIGHTEDGRAPHSET IMPLEMENTATIONS -----
template <typename T, typename U>
const U &WeightedGraphSet<T, U>::weight(std::size_t source,
                                        std::size_t target) const {
    assert(source < GraphSet<T>::size());
    assert(target < GraphSet<T>::size());

    return _weights[source][target];
}

template <typename T, typename U>
U &WeightedGraphSet<T, U>::weight(std::size_t source, std::size_t target) {
    assert(source < GraphSet<T>::size());
    assert(target < GraphSet<T>::size());

    return _weights[source][target];
}

template <typename T, typename U>
void WeightedGraphSet<T, U>::add_vertex(const T &label) {
    GraphSet<T>::add_vertex(label);
    _weights.emplace_back();
}

template <typename T, typename U>
bool WeightedGraphSet<T, U>::add_weight(std::size_t source, std::size_t target,
                                        const U &weight) {
    if(source < GraphSet<T>::size() && target < GraphSet<T>::size())
        _weights[source][target] = weight;

    return source < GraphSet<T>::size() && target < GraphSet<T>::size();
}

// ----- TRAVERSAL IMPLEMENTATIONS -----

template <typename F, typename G, typename S>
void rec_dfs(F f, G &g, S v, bool *marked) {
    set::Set<std::size_t> connections = g.neighbors(v);

    marked[v] = true;  // mark vertex v as visited
    f(g[v]);           // process v

    for(const auto &a : connections)              // loop neighbors set
        if(!marked[a]) rec_dfs(f, g, a, marked);  // recurse if not marked
}

template <typename F, typename G, typename S>
void depth_first(F f, G &g, S start) {
    bool *marked = new bool[g.size()];  // array of visited vertices
    assert(start < (S)g.size());

    std::fill_n(marked, g.size(), false);  // fill with false from 0 to size
    rec_dfs(f, g, start, marked);          // call rec_defs to process graph

    delete[] marked;
}

template <typename F, typename G, typename S>
void breadth_first(F f, G &g, S start) {
    bool *marked = new bool[g.size()];  // array of visited vertices
    assert(start < (S)g.size());

    set::Set<std::size_t> connections;       // neighbors set
    queue::Queue<std::size_t> vertex_queue;  // traversal queue

    std::fill_n(marked, g.size(), false);  // fill with false from 0 to size

    marked[start] = true;      // mark visited
    f(g[start]);               // process label
    vertex_queue.push(start);  // push vertex to queue for traversal

    do {
        // pop queue to get vertex and get neighbor's set of vertex
        connections = g.neighbors(vertex_queue.pop());

        // mark and process the unmarked neighbors, and place them in the queue.
        for(const auto &a : connections) {  // loop neighbors set
            if(!marked[a]) {                // if not marked
                marked[a] = true;           // mark
                f(g[a]);                    // then process label
                vertex_queue.push(a);       // then push vertex to queue
            }
        }
    } while(!vertex_queue.empty());

    delete[] marked;
}

template <typename T>
void print_label(T value) {
    std::cout << value << std::endl;
}

}  // namespace graph

#endif  // GRAPH_H
