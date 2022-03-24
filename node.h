#ifndef NODE_H
#define NODE_H

#include <string_view>
#include <unordered_map>
#include <memory>

#include "port.h"

using PortMap = std::unordered_map<std::string_view, std::unique_ptr<Port>>;

class Node
{
    // TODO: Implement class. A node should have a type and input/output ports that
    //       can be connected to build a node graph.

    static unsigned int nodesCreated;
    unsigned int id;
    std::string_view name;
    std::string_view type;
    PortMap inputPorts;
    PortMap outputPorts;

  public:
    Node(std::string_view n, std::string_view t);

    unsigned int readId() const;

    std::string_view readName() const;
  
    std::string_view readType() const;

    const PortMap& getPorts(PortType pt) const;

    PortMap& getPorts(PortType pt);

    bool anyConnections() const;
  
    void newPort(std::string_view portName, PortType pt);
    
    void connect(std::shared_ptr<Node>& other, std::string_view myPortName, std::string_view otherPortName, PortType myPortType);
  
    Port* findPort(std::string_view name, PortType pt);
};

#endif