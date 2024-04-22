#ifndef COMPOUND_H
#define COMPOUND_H

#include <unordered_map>
#include <unordered_set>

#include "node.h"
#include "hashNode.h"

// Type->Node Set. Can have nodes with the same name but diff type
using NodeMap = std::unordered_map<std::string_view, std::unordered_set<std::unique_ptr<Node>, HashNode, EqualToNode>>;

class Compound
{
    // TODO: Implement class. The compound should have the node graph.
   NodeMap nodes;
  
  public:
    const NodeMap& getAllNodes() const;

    Node* createNode(std::string_view name, std::string_view type);
};

// Utility functions
bool containsNodeType(const Compound& compound, std::string_view nodeType);
int getNodeCount(const Compound& compound);
int getMaxNumberOfConnectedInputPorts(const Compound& compound);
int getMinNumberOfOutputPorts(const Compound& compound);
int getNumberOfNodesWithNoConnections(const Compound& compound);

#endif