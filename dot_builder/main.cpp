#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

#include "static_info.pb.h"
#include "gb_llvm_types.hpp"
#include "dot_graph.hpp"

void generate_static_dot(const std::string& static_bin_path, const std::string& out_dot_path) {
    gb_ser::Graph static_graph;
    std::ifstream static_file(static_bin_path, std::ios::binary);
    
    if (!static_file) {
        throw std::runtime_error("Failed to open " + static_bin_path);
    }
    
    if (!static_graph.ParseFromIstream(&static_file)) {
        throw std::runtime_error("Failed to parse protobuf data");
    }

    gb::GraphBuilder builder;

    for (auto &pb_node : static_graph.nodes()) {
        uint64_t node_type = pb_node.type(); 
        gb::IdT node_id = pb_node.id();
        gb::INode *node = nullptr;
        switch (node_type) {
            case gb::Instr: node = builder.create_node<gb::Instr>(node_id); break;
            case gb::Constant: node = builder.create_node<gb::Constant>(node_id); break;
            default: throw std::runtime_error("Got unknown node type: '" + std::to_string(node_type) + "' during serialization.");
        }
        node->label() = pb_node.label();
    }
    for (auto &pb_edge : static_graph.edges()) {
        uint64_t edge_type = pb_edge.type();
        std::pair<gb::IdT, gb::IdT> edge_id = {pb_edge.left_id(), pb_edge.right_id()};
        gb::IEdge *edge = nullptr;
        switch (edge_type) {
            case gb::Flow: edge = builder.create_edge<gb::Flow>(edge_id); break;
            case gb::Data: edge = builder.create_edge<gb::Data>(edge_id); break;
            case gb::Call: edge = builder.create_edge<gb::Call>(edge_id); break;
            default: throw std::runtime_error("Got unknown edge type: '" + std::to_string(edge_type) + "' during serialization.");
        }
        edge->label() = pb_edge.label();
    }
    for (auto &pb_cluster : static_graph.clusters()) {
        uint64_t cluster_type = pb_cluster.type();
        gb::IdT cluster_id = pb_cluster.id();
        gb::ICluster *cluster = nullptr;
        switch (cluster_type) {
            case gb::BB: cluster = builder.create_cluster<gb::BB>(cluster_id); break;
            case gb::F:  cluster = builder.create_cluster<gb::F>(cluster_id);  break;
            default: throw std::runtime_error("Got unknown cluster type: '" + std::to_string(cluster_type) + "' during serialization.");
        }  
        cluster->label() = pb_cluster.label();
    }
    
    for (auto &pb_cluster : static_graph.clusters()) {
        gb::IdT cluster_id = pb_cluster.id();
        gb::IdT parent_id = pb_cluster.parent_id();
        gb::ICluster *cluster = builder.get_cluster(cluster_id);
        cluster->set_parent(builder.get_cluster(parent_id));
        for (gb::IdT node_id : pb_cluster.child_node_ids()) {
            builder.get_node(node_id)->set_parent(cluster);
        }
    }
    
    dot::DotGraph dot_graph(builder);

    std::ofstream out_file(out_dot_path);
    if (!out_file) {
        throw std::runtime_error("Failed to create " + out_dot_path);
    }
    
    dot_graph.serialize_dot(out_file);
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <static_info.bin> <output.dot>\n";
        return 1;
    }

    try {
        generate_static_dot(argv[1], "graph.dot");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}