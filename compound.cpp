#include "compound.h"
#include <climits>

 
const NodeMap& Compound::getAllNodes() const {
  return nodes;
}

Node* Compound::createNode(std::string_view name, std::string_view type) {
  auto p = nodes[type].insert(std::make_unique<Node>(name, type)); // still going to be a move regardless
  if (p.second) return p.first->get();
  return nullptr;
}

bool containsNodeType(const Compound& compound, std::string_view nodeType)
{
    // TODO: Return true if the compound contains a node of a given type, false
    //       otherwise. How would you optimize your search strategy if the graph
    //       contained a large number of connected nodes?
    const auto& allNodes = compound.getAllNodes();
    return allNodes.find(nodeType) != allNodes.end();
}

int getNodeCount(const Compound& compound)
{
    // TODO: Return the total number of nodes in the compound.
    const auto& allNodes = compound.getAllNodes(); 
    int res = 0;
    for (const auto&[nodeType, nodeSet] : allNodes) { // Structured-binding FTW!
      res += nodeSet.size();
    }
    return res;
}

int getMaxNumberOfConnectedInputPorts(const Compound& compound)
{
    // TODO: Return the maximum number of connected input ports found on any node in the compound.
    const auto& allNodes = compound.getAllNodes();
    int maxConnectedInputs = 0;
    for (const auto&[nodeType, nodeSet] : allNodes) {
      for (const auto& n : nodeSet) {
        if (n->inputPorts[1].size() > maxConnectedInputs)
          maxConnectedInputs = n->inputPorts[1].size();
      }
    }
    return maxConnectedInputs;
}

int getMinNumberOfOutputPorts(const Compound& compound)
{
    // TODO: Return the minimum number of output ports found on any node in the compound.
    const auto& allNodes = compound.getAllNodes();
    int minOutputs = INT_MAX;
    for (const auto&[nodeType, nodeSet] : allNodes) {
      for (const auto& n : nodeSet) {
        int numOutputs = n->getPorts(output).size();
        if (numOutputs < minOutputs) {
          minOutputs = numOutputs;
        }
      }
    }
    return minOutputs;
}


int getNumberOfNodesWithNoConnections(const Compound& compound)
{
    // TODO: Return the number of nodes that are not connected to any other node
    //       in the compound.
  int numDangling = 0;
  const auto& allNodes = compound.getAllNodes();
  for (const auto&[nodeType, nodeSet] : allNodes) {
    for (const auto& n : nodeSet) {
      if (!n->anyConnections()) {
        numDangling++;
      }
    }
  }
  return numDangling;
}