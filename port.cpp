#include "port.h"


// Hash Port
// template <> struct std::hash<Port> {
//     size_t operator()(const Port& key) const {
//         static const std::hash<std::string_view> h;
//         return h(key.getName()) ^ static_cast<size_t>(key.portType());
//     }
// };
// template <> struct std::equal_to<Port> {
//   bool operator()(const Port &lhs, const Port &rhs) const 
//   {
//       static const std::equal_to<Node> eq;
//       return lhs.getName() == rhs.getName() && lhs.portType() == rhs.portType();
//         // && eq(lhs.getOwner(), rhs.getOwner());
//   }
// };
// template <> struct std::hash<std::reference_wrapper<Port>> {
//     size_t operator()(const std::reference_wrapper<Port>& key) const {
//         static const std::hash<Port> portH; 
//         const Port& k = key.get();
//         return portH(k);
//     }
// };
// template <> struct std::equal_to<std::reference_wrapper<const Port>> {
//   bool operator()(const std::reference_wrapper<Port> &lhs, const std::reference_wrapper<Port> &rhs) const 
//   {
//       static const std::equal_to<Port> portEq; 

//       const Port& l = lhs.get();
//       const Port& r = rhs.get();

//       return portEq(l, r);
//   }
// };

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
