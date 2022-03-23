#include <iostream>
#include <string>
#include <string_view>
#include <climits>
#include <functional>
#include <vector>
// #include <optional>
#include <unordered_set>


class Node; // forward declare

enum PortType {input, output};

class Port
{
    // TODO: Implement class. The port should be able to know if it's an input or
    //       an output type.
  std::string name;
  PortType type;
  std::reference_wrapper<Node> owner;
  std::vector<std::reference_wrapper<Port>> connections;
  
  public:
    Port(Node& o, std::string_view n, PortType t) : name(n), type(t), owner(o) {
    }
    
  
    std::string_view getName() const { return name; }
  
  
    const Node& readOwner() const { return owner.get(); }
  
  
    const PortType portType() const { return type; }
  
  
    const std::vector<std::reference_wrapper<Port>>& readConnections() const {
      return connections;
    }
  
    bool isConnected() const {
      return connections.size() > 0;
    }
    
  
    void connect(Port& p) {
      if (p.type != this->type) {
        p.connections.push_back(std::ref(*this));
        connections.push_back(std::ref(p));
      }
    }


    // CRUCIAL for hashing (detects collisions)
    bool operator==(const Port& o) const {
        return name == o.name && type == o.type;
    }
};

// Hash port name & type
template <> struct std::hash<Port> {
    size_t operator()(const Port& key) const {
        static const std::hash<std::string_view> h;
        return h(key.getName()) + static_cast<std::size_t>(key.portType());
    }
};


class Node
{
    // TODO: Implement class. A node should have a type and input/output ports that
    //       can be connected to build a node graph.
    std::string name;
    std::string type;
    std::vector<Port> inputPorts;
    std::vector<Port> outputPorts;
  
  
  public:
    Node(std::string_view n, std::string_view t) : name(n), type(t) { }
  
    
    std::string_view readName() const { return name; }
  
  
    std::string_view readType() const { return type; }
  
  
    const std::vector<Port>& readPorts(PortType pt) const {
      if (pt == input) return inputPorts;
      else return outputPorts;
    }
  
    
    bool anyConnections() const {
      bool connected = false;
      
      for (const auto& p : inputPorts) {
        connected = p.isConnected();
        if (connected) break;
      }
      
      if (!connected) {
        for (const auto& p : outputPorts) {
          connected = p.isConnected();
          if (connected) break;
        }
      }
      
      return connected;
    }
  
  
    void newPort(std::string_view portName, PortType pt) {
      Port p(*this, portName, pt);
      
      if (pt == input) inputPorts.push_back(p);
      else outputPorts.push_back(p);
    }
    
  
    void connect(Node& other, std::string_view myPortName, std::string_view otherPortName, PortType myPortType) {
      Port* p = findPort(myPortName, myPortType);
      if (p == nullptr) return;
      
      Port* op = other.findPort(otherPortName, myPortType == input? output : input);
      if (op == nullptr) return;
      
      p->connect(*op);
    }
  

    // CRUCIAL for hashing (detects collisions)
    bool operator==(const Node& o) const {
        return name == o.name && type == o.type;
    }
  

  private:
      Port* findPort(std::string_view name, PortType pt) {
      std::vector<Port>& ports = pt == input? inputPorts: outputPorts;
      
      for (auto& p : ports) {
        if (p.getName() == name) {
          return &p;
        }
      }
      
      return nullptr;
    }
};


// Hash node name & type
template <> struct std::hash<Node> {
    size_t operator()(const Node& key) const {
        static const std::hash<std::string_view> h;
        return h(key.readName()) + h(key.readType());
    }
};


class Compound
{
    // TODO: Implement class. The compound should have the node graph.
  std::unordered_set<Node> nodes;
  
  public:
    std::unordered_set<Node> getNodes() {
      return nodes;
    }
    
  
    void insertNode(Node newNode) {
      nodes.insert(newNode);
    }
};

bool containsNodeType(Compound compound, std::string nodeType)
{
    // TODO: Return true if the compound contains a node of a given type, false
    //       otherwise. How would you optimize your search strategy if the graph
    //       contained a large number of connected nodes?
  
    // FIXME: have a hash-map for different nodeTypes
    for (const auto& n : compound.getNodes()) {
      if (n.readType() == nodeType) {
        return true;
      }
    }
  
    return false;
}

int getNodeCount(Compound compound)
{
    // TODO: Return the total number of nodes in the compound.
  return compound.getNodes().size();
}

int getMaxNumberOfConnectedInputPorts(Compound compound)
{
    // TODO: Return the maximum number of ports found on any node in the compound.
  int maxInputs = 0;
  for (const auto& n: compound.getNodes()) {
    int numInputs = n.readPorts(input).size();
    if (numInputs > maxInputs) {
      maxInputs = numInputs;
    }
  }
  return maxInputs;
}

int getMinNumberOfOutputPorts(Compound compound)
{
    // TODO: Return the minimum number of ports found on any node in the compound.
  int minOutputs = INT_MAX;
  for (const auto& n: compound.getNodes()) {
    int numOutputs = n.readPorts(output).size();
    if (numOutputs < minOutputs) {
      minOutputs = numOutputs;
    }
  }
  return minOutputs;
}


int getNumberOfNodesWithNoConnections(Compound compound)
{
    // TODO: Return the number of nodes that are not connected to any other node
    //       in the compound.
  int numDangling = 0;
  
  for (const auto& n: compound.getNodes()) {
    if (!n.anyConnections()) {
      numDangling++;
    }
  }
  
  return numDangling;
}

int main()
{
    // -- TEST SETUP --
    // Create a test compound.
    Compound compound;

    // TODO: Populate the compound with the necessary nodes and ports to pass the
    //       tests below.
    Node my_surface_material("my_surface_material", "standard_surface");
    my_surface_material.newPort("transmission_color", input);
    my_surface_material.newPort("transmission_scatter", input);
    my_surface_material.newPort("base_color", input);
    my_surface_material.newPort("dangling_output", output);
  
    Node my_uv_projection("my_uv_projection", "uv_projection");
    my_uv_projection.newPort("color", output);
    my_uv_projection.newPort("projection_color", input);
    my_uv_projection.connect(my_surface_material, "color", "transmission_color", output);
    my_uv_projection.connect(my_surface_material, "color", "transmission_scatter", output);
    
  
    Node transmission_texture("transmission_texture", "image");
    transmission_texture.newPort("color", output);
    transmission_texture.connect(my_uv_projection, "color", "projection_color", output);
  
  
    Node base_texture("base_texture", "image");
    base_texture.newPort("color", output);
    base_texture.connect(my_surface_material, "color", "base_color", output);
  
  
    compound.insertNode(my_surface_material);
    compound.insertNode(my_uv_projection);
    compound.insertNode(transmission_texture);
    compound.insertNode(base_texture);

    // -- RUN TESTS --
    // Ensure the test node graph has a node of type standard_surface.
    bool test1 = containsNodeType(compound, "uv_projection");

    // We are expecting the node graph to consists of 4 connected nodes.
    bool test2 = getNodeCount(compound) == 4;

    // No node has more than 3 connected inputs.
    bool test3 = getMaxNumberOfConnectedInputPorts(compound) == 3;

    // All the nodes in the node graph have at least 1 output port.
    bool test4 = getMinNumberOfOutputPorts(compound) == 1;
    
    // Every node in the node graph is connected to at least one node in the graph.
    bool test5 = getNumberOfNodesWithNoConnections(compound) == 0;
    
    std::cout << "test1: " << test1 << '\n';
    std::cout << "test2: " << test2 << '\n';
    std::cout << "test3: " << test3 << '\n';
    std::cout << "test4: " << test4 << '\n';
    std::cout << "test5: " << test5 << '\n';

    return 0;
}