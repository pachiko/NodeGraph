#ifndef PORT_H
#define PORT_H

#include <functional>
#include <unordered_set>
#include "hashPort.h"

enum PortType {input, output};
class Node;

using PortConnections = std::unordered_set<std::reference_wrapper<Port>, HashPort, EqualToPort>;

class Port
{
    // TODO: Implement class. The port should be able to know if it's an input or
    //       an output type.
  const Node& owner;
  std::string_view name;
  PortType type;
  PortConnections connections;

  public:
    Port(const Node& o, std::string_view n, PortType t);

    ~Port();

    const Node & getOwner() const;

    std::string_view getName() const;
  
    const PortType portType() const;
  
    const PortConnections& getConnections() const;
  
    bool isConnected() const;

    void disconnect(Port& p);
    
    void connect(Port& p);
};

#endif