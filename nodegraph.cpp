#include <iostream>
#include <string_view>
#include <memory>


#include "port.h"
#include "node.h"
#include "compound.h"

int main()
{
    // -- TEST SETUP --
    // Create a test compound.
    Compound compound;

    // TODO: Populate the compound with the necessary nodes and ports to pass the
    //       tests below.

    // NB: String literals have program lifetime
    Node* my_surface_material = compound.createNode("my_surface_material", "standard_surface");
    my_surface_material->newPort("transmission_color", input);
    my_surface_material->newPort("transmission_scatter", input);
    my_surface_material->newPort("base_color", input);
    my_surface_material->newPort("dangling_output", output);
  

    Node* my_uv_projection = compound.createNode("my_uv_projection", "uv_projection");
    my_uv_projection->newPort("color", output);
    my_uv_projection->newPort("projection_color", input);
    my_uv_projection->connect(my_surface_material, "color", "transmission_color", output);
    my_uv_projection->connect(my_surface_material, "color", "transmission_scatter", output);
    
  
    Node* transmission_texture = compound.createNode("transmission_texture", "image");
    transmission_texture->newPort("color", output);
    transmission_texture->connect(my_uv_projection, "color", "projection_color", output);
  
  
    Node* base_texture = compound.createNode("base_texture", "image");
    base_texture->newPort("color", output);
    base_texture->newPort("color", output); // can't create duplicate port
    base_texture->connect(my_surface_material, "color", "base_color", output);
    base_texture->connect(my_surface_material, "color", "base_color", output); // can't connect to same port again
    // can connect to another port instance even if existing connection has same name & type
    my_surface_material->connect(my_uv_projection, "base_color" , "color", input);

    // no duplicates
    Node* abc = compound.createNode("my_surface_material", "standard_surface");
    bool test0 = abc == nullptr;

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

    // Test the disconnect() function
    transmission_texture->disconnect(my_uv_projection, "color", "projection_color", output);
    bool test6 = getNumberOfNodesWithNoConnections(compound) == 1;
    
    std::cout << "test0: " << test0 << '\n';
    std::cout << "test1: " << test1 << '\n';
    std::cout << "test2: " << test2 << '\n';
    std::cout << "test3: " << test3 << '\n';
    std::cout << "test4: " << test4 << '\n';
    std::cout << "test5: " << test5 << '\n';
    std::cout << "test6: " << test6 << '\n';

    return 0;
}