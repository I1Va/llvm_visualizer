#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include "static_info.pb.h"
#include "dynamic_info.pb.h"

#include "graph_builder.hpp"

void build_dot(const std::string& static_path, const std::string& dynamic_path) {
    gb_ser::Graph static_graph;
    std::ifstream static_file(static_path, std::ios::binary);
    if (!static_graph.ParseFromIstream(&static_file)) {
        throw std::runtime_error("Failed to parse static info");
    }

    instrumentation::ExecutionData dyn_data;
    std::ifstream dyn_file(dynamic_path, std::ios::binary);
    if (!dyn_data.ParseFromIstream(&dyn_file)) {
        throw std::runtime_error("Failed to parse dynamic info");
    }

    std::map<uint64_t, uint64_t> counts = {dyn_data.bb_counts().begin(), dyn_data.bb_counts().end()};

    std::cout << "digraph G {\n";
    std::cout << "  node [shape=box, style=filled, fontname=\"Courier\"];\n";

    for (const auto& cluster : static_graph.clusters()) {
        std::cout << "  subgraph cluster_" << cluster.id() << " {\n";
        std::cout << "    label = \"" << cluster.label() << "\";\n";
        std::cout << "    color = blue;\n";
        
        for (uint64_t node_id : cluster.child_node_ids()) {
            std::cout << "    node_" << node_id << ";\n";
        }
        std::cout << "  }\n";
    }

    for (const auto& node : static_graph.nodes()) {
        uint64_t exec_count = counts.count(node.id()) ? counts.at(node.id()) : 0;
        
        std::string color = (exec_count > 0) ? "lightyellow" : "gray90";
        if (exec_count > 100) color = "orange";
        if (exec_count > 1000) color = "indianred1";

        std::cout << "  node_" << node.id() << " [label=\"" << node.label() 
                  << "\\nHits: " << exec_count << "\", fillcolor=" << color << "];\n";
    }

    for (const auto& edge : static_graph.edges()) {
        std::cout << "  node_" << edge.left_id() << " -> node_" << edge.right_id() 
                  << " [label=\"" << edge.label() << "\"];\n";
    }

    std::cout << "}\n";
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <static.bin> <dynamic.bin>\n";
        return 1;
    }

    try {
        build_dot(argv[1], argv[2]);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}