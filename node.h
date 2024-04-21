#ifndef NODE_H
#define NODE_H

#include <string_view>
#include <unordered_map>
#include <memory>

#include "port.h"

using PortMap = std::unordered_map<std::string_view, std::unique_ptr<Port>>;
using PortCache = std::unordered_map<std::string_view, Port*>;

class Node
{
    // TODO: Implement class. A node should have a type and input/output ports that
    //       can be connected to build a node graph.

    static unsigned int nodesCreated;
    unsigned int id;

    std::string_view name;
    std::string_view type;

    void portConnectionChanged(Port* p);

  public:
    // NO duplicate ports
    PortMap inputPorts[2]; // 0 = unconnected, 1 = connected
    PortCache allInputPorts; // cached for quick access.

    PortMap outputPorts[2]; // 0 = unconnected, 1 = connected
    PortCache allOutputPorts; // cached for quick access.

    Node(std::string_view n, std::string_view t);

    unsigned int getId() const;

    std::string_view getName() const;
  
    std::string_view getType() const;

    const PortCache& getPorts(PortType pt) const;

    bool anyConnections() const;
  
    void newPort(std::string_view portName, PortType pt);
    
    void connect(std::shared_ptr<Node>& other, std::string_view myPortName, std::string_view otherPortName, PortType myPortType);

    void disconnect(std::shared_ptr<Node>& other, std::string_view myPortName, std::string_view otherPortName, PortType myPortType);
  
    Port* findPort(std::string_view name, PortType pt);
};

#endif