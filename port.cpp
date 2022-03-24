#include "port.h"


Port::Port(const Node& o, std::string_view n, PortType t) : owner(o), name(n), type(t) {
}

const Node& Port::getOwner() const { return owner; }

std::string_view Port::getName() const { return name; }

const PortType Port::portType() const { return type; }

const std::vector<std::reference_wrapper<Port>>& Port::readConnections() const {
  return connections;
}

bool Port::isConnected() const {
  return connections.size() > 0;
}

void Port::connect(Port& p) {
  if (p.type != this->type) {
    p.connections.push_back(std::ref(*this));
    connections.push_back(std::ref(p));
  }
}
