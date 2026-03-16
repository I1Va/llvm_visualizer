#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

#include "static_info.pb.h"
#include "dot_builder.hpp" 

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
        builder.create_cluster<dot::BBCluster>(pb_cluster.id(), pb_cluster.label()); // FIX, add FClusters
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

    // for (const auto& pb_node : static_graph.nodes()) {
    //     if (pb_node.parent_id() != 0) {
    //         dot::ICluster* parent_cluster = builder.get_cluster_by_id(pb_node.parent_id());
    //         if (parent_cluster) {
    //             builder.create_node_in_cluster<dot::InstrNode>(pb_node.id(), *parent_cluster, pb_node.label());
    //         } else {
    //             builder.create_node<dot::InstrNode>(pb_node.id(), pb_node.label());
    //         }
    //     } else {
    //         builder.create_node<dot::InstrNode>(pb_node.id(), pb_node.label());
    //     }
    // }

    // for (const auto& pb_edge : static_graph.edges()) {
    //     dot::INode* left_node = builder.get_node_by_id(pb_edge.left_id());
    //     dot::INode* right_node = builder.get_node_by_id(pb_edge.right_id());
    //     dot::ICluster* left_cluster = builder.get_cluster_by_id(pb_edge.left_id());
    //     dot::ICluster* right_cluster = builder.get_cluster_by_id(pb_edge.right_id());

    //     if (left_node && right_node) {
    //         builder.create_edge<dot::FlowEdge>(*left_node, *right_node, pb_edge.label());
    //     } else if (left_node && right_cluster) {
    //         builder.create_edge<dot::FlowEdge>(*left_node, *right_cluster, pb_edge.label());
    //     } else if (left_cluster && right_node) {
    //         builder.create_edge<dot::FlowEdge>(*left_cluster, *right_node, pb_edge.label());
    //     } else if (left_cluster && right_cluster) {
    //         builder.create_edge<dot::FlowEdge>(*left_cluster, *right_cluster, pb_edge.label());
    //     }
    // }

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