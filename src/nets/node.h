//
// Created by shahnoor on 8/9/18.
//

#ifndef NETWORK_NODE_H
#define NETWORK_NODE_H

#include <vector>
#include <ostream>

/**
 * Node of a network
 */
class Node{
    size_t  _id;
    std::vector<size_t> _connected_nodes;

    int _group_id;

    // at i-th iteration m nodes are linked.
    // if nodes have same _current_group value that means the node is already selected
    size_t _current_group{};
public:
    ~Node() {_connected_nodes.clear();};
    Node(size_t id);
    size_t get_id() const { return  _id;}
    int get_group_id() const { return  _group_id;}
    void set_group_id(int id) {_group_id = id;}

    size_t get_current_group() const {return _current_group;}
    void set_current_group(size_t grp) {_current_group = grp;}

    const std::vector<size_t>& get_neighbors() const {return _connected_nodes;}

    void add_neighbor(Node n);
    void add_neighbor(size_t n);

    /**
     * Checked version
     * less efficient but useful for debugging
     */
    void add_neighbor_checked(Node n);
    void add_neighbor_checked(size_t n);

    /**
     * Number of links a node have is the degree of that node
     */
    size_t degree() const {return _connected_nodes.size();}

};


std::ostream& operator<<(std::ostream& os, const Node& node);

#endif //NETWORK_NODE_H
