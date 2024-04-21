#include "node.h"

unsigned int Node::nodesCreated{ 0 };

Node::Node(std::string_view n, std::string_view t) : id(++nodesCreated), name(n), type(t) { }

unsigned int Node::getId() const { return id; }

std::string_view Node::getName() const { return name; }

std::string_view Node::getType() const { return type; }

const PortCache& Node::getPorts(PortType pt) const {
  return pt == input ? allInputPorts : allOutputPorts;
}

bool Node::anyConnections() const {    
  return !inputPorts[1].empty() || !outputPorts[1].empty();
}

void Node::newPort(std::string_view portName, PortType pt) {
  PortMap& pm = pt == input ? inputPorts[0] : outputPorts[0];
  pm.insert(std::make_pair(portName, std::make_unique<Port>(*this, portName, pt)));

  PortCache& pc = pt == input ? allInputPorts : allOutputPorts;
  pc.insert(std::make_pair(portName, pm[portName].get()));
}

void Node::connect(std::shared_ptr<Node>& other, std::string_view myPortName, std::string_view otherPortName, PortType myPortType) {
  Port* p = findPort(myPortName, myPortType);
  if (p == nullptr) return;
  
  Port* op = other->findPort(otherPortName, myPortType == input? output : input);
  if (op == nullptr) return;
  
  p->connect(*op);
  portConnectionChanged(p);
  other->portConnectionChanged(op);
}

void Node::disconnect(std::shared_ptr<Node>& other, std::string_view myPortName, std::string_view otherPortName, PortType myPortType) {
  Port* p = findPort(myPortName, myPortType);
  if (p == nullptr) return;
  
  Port* op = other->findPort(otherPortName, myPortType == input? output : input);
  if (op == nullptr) return;

  p->disconnect(*op);
  portConnectionChanged(p);
}

Port* Node::findPort(std::string_view name, PortType pt) {
  PortCache& pc = pt == input ? allInputPorts : allOutputPorts;
  auto it = pc.find(name);
  if (it != pc.end()) {
    return it->second;
  }
  return nullptr;
}

void Node::portConnectionChanged(Port* p) {
  std::string_view name = p->getName();
  PortType type = p->portType();

  size_t i = p->isConnected() ? 1 : 0; // target map
  size_t o = i ^ 1; // opposite map.

  PortMap& target = type == input ? inputPorts[i] : outputPorts[i];
  PortMap& opposite = type == input ? inputPorts[o] : outputPorts[o];

  if (target.find(name) == target.end()) { // need to change map.
    auto elem = opposite.extract(name);
    target.insert(std::move(elem));
  }
}