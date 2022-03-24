#include "node.h"

unsigned int Node::nodesCreated{ 0 };

Node::Node(std::string_view n, std::string_view t) : id(++nodesCreated), name(n), type(t) { }

unsigned int Node::readId() const { return id; }

std::string_view Node::readName() const { return name; }

std::string_view Node::readType() const { return type; }

const std::unordered_map<std::string_view, std::unique_ptr<Port>>& Node::getPorts(PortType pt) const {
  return pt == input? inputPorts : outputPorts;
}

std::unordered_map<std::string_view, std::unique_ptr<Port>>& Node::getPorts(PortType pt) {
  return const_cast< std::unordered_map<std::string_view, std::unique_ptr<Port>>& >
    (const_cast<const Node*>(this)->getPorts(pt));
}

bool Node::anyConnections() const {    
  for (const auto& [k, p] : inputPorts) {
      if (p->isConnected()) {
        return true;
      }
  }
  
  for (const auto& [k, p] : outputPorts) {
      if (p->isConnected()) {
        return true;
      }
  }

  return false;
}

void Node::newPort(std::string_view portName, PortType pt) {
  getPorts(pt).insert(std::make_pair(portName, std::make_unique<Port>(*this, portName, pt)));
}

void Node::connect(std::shared_ptr<Node>& other, std::string_view myPortName, std::string_view otherPortName, PortType myPortType) {
  Port* p = findPort(myPortName, myPortType);
  if (p == nullptr) return;
  
  Port* op = other->findPort(otherPortName, myPortType == input? output : input);
  if (op == nullptr) return;
  
  p->connect(*op);
}

Port* Node::findPort(std::string_view name, PortType pt) {
  std::unordered_map<std::string_view, std::unique_ptr<Port>>& ports{getPorts(pt)};
  auto it = ports.find(name);
  if (it != ports.end()) {
    return it->second.get();
  }
  return nullptr;
}