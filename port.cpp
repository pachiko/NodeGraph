#include "port.h"


Port::Port(const Node& o, std::string_view n, PortType t) : owner(o), name(n), type(t) {
}

Port::~Port() {
  for (auto& op: connections) {
    Port& opr = op.get();
    PortConnections& oc = opr.connections;
    oc.erase(std::ref(*this));
  }
}

const Node& Port::getOwner() const { return owner; }

std::string_view Port::getName() const { return name; }

const PortType Port::portType() const { return type; }

const PortConnections& Port::getConnections() const {
  return connections;
}

void Port::disconnect(Port& p) {
  PortConnections& oc = p.connections;
  oc.erase(std::ref(*this));

  connections.erase(p);
}

bool Port::isConnected() const {
  return connections.size() > 0;
}

void Port::connect(Port& p) {
  if (p.type != this->type) {
    p.connections.insert(std::ref(*this));
    connections.insert(std::ref(p));
  }
}
