#ifndef HASH_NODE_H
#define HASH_NODE_H

// Nodes cannot have the same name and type within the same Compound
#include "node.h"

static const std::hash<std::string_view> h;

struct HashNode {
    size_t operator()(const Node& n) const
    {
        return h(n.readName()) ^ (h(n.readType()) << 1);
    }

    size_t operator()(const std::shared_ptr<Node>& n) const
    {   
        return this->operator()(*n);
    }
};

struct EqualToNode {
    bool operator()(const Node &lhs, const Node &rhs) const 
    {
        return lhs.readName() == rhs.readName() && lhs.readType() == rhs.readType();
    }

    bool operator()(const std::shared_ptr<Node> &lhs, const std::shared_ptr<Node> &rhs) const 
    {
        return this->operator()(*lhs, *rhs);
    }
};

#endif