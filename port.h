#ifndef PORT_H
#define PORT_H

#include <functional>
#include <unordered_set>
#include <vector>

enum PortType {input, output};
class Node;

class Port
{
    // TODO: Implement class. The port should be able to know if it's an input or
    //       an output type.
  const Node& owner;
  std::string_view name;
  PortType type;
  std::vector<std::reference_wrapper<Port>> connections;

  public:
    Port(const Node& o, std::string_view n, PortType t);

    const Node & getOwner() const;

    std::string_view getName() const;
  
    const PortType portType() const;
  
    const std::vector<std::reference_wrapper<Port>>& readConnections() const;
  
    bool isConnected() const;
    
    void connect(Port& p);
};

#endif