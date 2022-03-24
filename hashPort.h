#ifndef HASH_PORT_H
#define HASH_PORT_H

// Ports can connect to multiple ports of the same name and type (but from different nodes)
// Checks pointers for collision

class Port;
static const std::hash<Port *> ptrH;

struct HashPort {
    size_t operator()(const std::reference_wrapper<Port>& p) const
    {   
        return ptrH(std::addressof(p.get()));
    }
};

struct EqualToPort {
    bool operator()(const std::reference_wrapper<Port> &lhs, const std::reference_wrapper<Port> &rhs) const 
    {
        return ptrH(std::addressof(lhs.get())) == ptrH(std::addressof(rhs.get()));
    }
};

#endif