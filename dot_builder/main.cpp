#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

#include "static_info.pb.h"
#include "dot_builder.hpp" 
#include "gb_llvm_types.hpp"

dot::ICluster* create_cluster(const gb_ser::Cluster &cluster, dot::DotBuilder &builder) {
    uint64_t cluster_type = cluster.type();
    switch (cluster_type) {
        case gb::ClusterTypes::F: 
            return builder.create_cluster<dot::FCluster>(cluster.id(), cluster.label());
            
        case gb::ClusterTypes::BB: 
            return builder.create_cluster<dot::BBCluster>(cluster.id(), cluster.label());
        default:
            throw std::runtime_error("unknown cluster type : " + std::to_string(cluster_type));
    }
}

dot::INode* create_node(const gb_ser::Node &node, dot::DotBuilder &builder) {
    uint64_t node_type = node.type();
    switch (node_type) {
        case gb::NodeTypes::Instr: 
            return builder.create_node<dot::InstrNode>(node.id(), node.label());
    
        case gb::NodeTypes::Value: 
            return builder.create_node<dot::ValueNode>(node.id(), node.label());
        default:
            throw std::runtime_error("unknown node type : " + std::to_string(node_type));
    }
}

dot::INode* create_node_in_cluster(const gb_ser::Node &node, dot::ICluster &parent, dot::DotBuilder &builder) {
    uint64_t node_type = node.type();
    switch (node_type) {
        case gb::NodeTypes::Instr: 
            return builder.create_node_in_cluster<dot::InstrNode>(node.id(), parent, node.label());
    
        case gb::NodeTypes::Value: 
            return builder.create_node_in_cluster<dot::ValueNode>(node.id(), parent, node.label());
        default:
            throw std::runtime_error("unknown node type : " + std::to_string(node_type));
    }
}

dot::IEdge* create_edge(const gb_ser::Edge &edge, dot::DotBuilder &builder) {
    dot::INode* left_node = builder.get_node_by_id(edge.left_id());
    dot::INode* right_node = builder.get_node_by_id(edge.right_id());
    dot::ICluster* left_cluster = builder.get_cluster_by_id(edge.left_id());
    dot::ICluster* right_cluster = builder.get_cluster_by_id(edge.right_id());

    uint64_t edge_type = edge.type();
    auto construct_edge = [&](auto& source, auto& target) -> dot::IEdge* {
        switch (edge_type) {
            case gb::EdgeTypes::Flow: 
                return builder.create_edge<dot::FlowEdge>(source, target, edge.label());
            case gb::EdgeTypes::Data: 
                return builder.create_edge<dot::DataEdge>(source, target, edge.label());
            case gb::EdgeTypes::Call: 
                return builder.create_edge<dot::CallEdge>(source, target, edge.label());
            default:
                throw std::runtime_error("Unknown edge type: " + std::to_string(edge_type));
        }
    };

    if (left_node && right_node)           return construct_edge(*left_node, *right_node);
    if (left_node && right_cluster)        return construct_edge(*left_node, *right_cluster);
    if (left_cluster && right_node)        return construct_edge(*left_cluster, *right_node);
    if (left_cluster && right_cluster)     return construct_edge(*left_cluster, *right_cluster);

    throw std::runtime_error("Edge source/target not found for IDs: " + 
                             std::to_string(edge.left_id()) + " -> " + 
                             std::to_string(edge.right_id()));
}

void generate_static_dot(const std::string& static_bin_path, const std::string& out_dot_path) {
    gb_ser::Graph static_graph;
    std::ifstream static_file(static_bin_path, std::ios::binary);
    
    if (!static_file) {
        throw std::runtime_error("Failed to open " + static_bin_path);
    }
    
    if (!static_graph.ParseFromIstream(&static_file)) {
        throw std::runtime_error("Failed to parse protobuf data");
    }

    dot::DotBuilder builder;

    for (const auto& pb_cluster : static_graph.clusters()) {
        create_cluster(pb_cluster, builder);
    }

    for (const auto& pb_cluster : static_graph.clusters()) {
        if (pb_cluster.parent_id() != 0) {
            dot::ICluster* cluster = builder.get_cluster_by_id(pb_cluster.id());
            dot::ICluster* parent = builder.get_cluster_by_id(pb_cluster.parent_id());
            if (cluster && parent) {
                cluster->set_parent(parent);
            }
        }
    }


    for (const auto& pb_node : static_graph.nodes()) {
        if (pb_node.parent_id() != 0) {
            dot::ICluster* parent_cluster = builder.get_cluster_by_id(pb_node.parent_id());
            if (parent_cluster) {
                create_node_in_cluster(pb_node, *parent_cluster, builder);
            } else {
                create_node(pb_node, builder);
            }
        } else {
            builder.create_node<dot::InstrNode>(pb_node.id(), pb_node.label());
            create_node(pb_node, builder);
        }
    }

    for (const auto& pb_edge : static_graph.edges()) {
        create_edge(pb_edge, builder);
    }

    std::ofstream out_file(out_dot_path);
    if (!out_file) {
        throw std::runtime_error("Failed to create " + out_dot_path);
    }
    
    builder.serialize_dot(out_file);
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