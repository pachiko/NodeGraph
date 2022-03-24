#ifndef HASH_PORT_H
#define HASH_PORT_H

#include "hashNode.h"
#include "port.h"

struct HashPort {
    size_t operator()(const Port& p) const
    {
        return h(p.getName()) ^ static_cast<size_t>(p.portType());
    }

    size_t operator()(const std::reference_wrapper<Port>& p) const
    {   
        return this->operator()(p.get());
    }
};

struct EqualToPort {
    bool operator()(const Port &lhs, const Port &rhs) const 
    {
        return lhs.getName() == rhs.getName() && lhs.portType() == rhs.portType();
    }

    bool operator()(const std::reference_wrapper<Port> &lhs, const std::reference_wrapper<Port> &rhs) const 
    {
        return this->operator()(lhs.get(), rhs.get());
    }
};

#endif