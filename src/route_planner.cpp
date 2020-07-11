#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.
    this->start_node = &(m_Model.FindClosestNode(start_x, start_y));
    // std::cout << "Start node : " << this->start_node->x << " " << this->start_node->y << "\n";

    this->end_node = &(m_Model.FindClosestNode(end_x, end_y));
    // std::cout << "End node : " << this->end_node->x << " " << this->end_node->y << "\n";
}

// - Call distance method of Node to determine the distance to another node.
float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    float dist = node->distance(*(this->end_node));
    // std::cout << "Distance to other node : " << dist << "\n";
    return dist; 
}

// - For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.
void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
    current_node->FindNeighbors();
    for(auto node : current_node->neighbors) {
        node->parent = current_node;
        node->h_value = this->CalculateHValue(node);
        node->g_value = node->distance(*(current_node)) + current_node->g_value;
        node->visited = true;
        this->open_list.push_back(node); 
    }
}

// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.
RouteModel::Node *RoutePlanner::NextNode() {
    auto compare = [](RouteModel::Node *one, RouteModel::Node *two) {return (one->h_value+one->g_value) > (two->g_value+two->h_value);};
    std::sort(this->open_list.begin(), this->open_list.end(), compare);
    return this->open_list.back();
}

// - This method takes the final node as an argument and iteratively follow the 
//   chain of parents of nodes until the starting node is found.
// - For each node in the chain, add the distance from the node to its parent to the distance variable.
// - The returned vector should be in the correct order: the start node should be the first element
//   of the vector, the end node should be the last element.
std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    path_found.push_back(*(current_node));
    RouteModel::Node *temp_node = current_node;
    while (temp_node->parent != nullptr) {
        path_found.push_back(*(temp_node->parent));
        distance = distance + temp_node->distance(*(temp_node->parent));
        temp_node = temp_node->parent;
    }
    std::reverse(path_found.begin(), path_found.end());

    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}

// - Use the AddNeighbors method to add all of the neighbors of the current node to the open_list.
// - Use the NextNode() method to sort the open_list and return the next node.
// - When the search has reached the end_node, use the ConstructFinalPath method to return the final path that was found.
// - Store the final path in the m_Model.path attribute before the method exits. This path will then be displayed on the map tile.
void RoutePlanner::AStarSearch() {
    RouteModel::Node *current_node = nullptr;

    // set current node to be start node 
    current_node = this->start_node;

    if (current_node == this->end_node) {
        std::cout << "Found Path!" << std::endl;
        m_Model.path = this->ConstructFinalPath(current_node);
    }

    this->open_list.push_back(current_node);
    while (!open_list.empty())
    {
        /* code */
    }
    

}