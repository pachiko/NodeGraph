#ifndef COMPOUND_H
#define COMPOUND_H

#include <unordered_map>
#include <unordered_set>

#include "node.h"
#include "hashNode.h"

using NodeMap = std::unordered_map<std::string_view, std::unordered_set<std::shared_ptr<Node>, HashNode, EqualToNode>>;

class Compound
{
    // TODO: Implement class. The compound should have the node graph.
   NodeMap nodes;
  
  public:
    const NodeMap& getAllNodes() const;

    void insertNode(const std::shared_ptr<Node>& newNode);
};

// Utility functions
bool containsNodeType(const Compound& compound, std::string_view nodeType);
int getNodeCount(const Compound& compound);
int getMaxNumberOfConnectedInputPorts(const Compound& compound);
int getMinNumberOfOutputPorts(const Compound& compound);
int getNumberOfNodesWithNoConnections(const Compound& compound);

#endif