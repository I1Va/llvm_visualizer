#pragma once

#include <google/protobuf/stubs/common.h>
#include <fstream>

#include "graph_builder.hpp"
#include "gb_llvm_types.hpp"
#include "static_info.pb.h"

namespace proto {

class GraphSerializer {
public:
    static int serialize(const gb::GraphBuilder &builder, const std::string &filename) {
        GOOGLE_PROTOBUF_VERIFY_VERSION;
        gb_ser::Graph proto_graph;

        for (const auto &[id, node_ptr] : builder.nodes()) {
            auto *p_node = proto_graph.add_nodes();
            p_node->set_id(node_ptr->id());
            p_node->set_type(node_ptr->type());
            p_node->set_label(node_ptr->label());
            if (node_ptr->parent()) {
                p_node->set_parent_id(node_ptr->parent()->id());
            }
        }

        for (const auto &[id, cluster_ptr] : builder.clusters()) {
            auto *p_cluster = proto_graph.add_clusters();
            p_cluster->set_id(cluster_ptr->id());
            p_cluster->set_type(cluster_ptr->type());
            p_cluster->set_label(cluster_ptr->label());
            
            if (cluster_ptr->parent()) {
                p_cluster->set_parent_id(cluster_ptr->parent()->id());
            }
            
            for (auto *node : cluster_ptr->nodes()) {
                p_cluster->add_child_node_ids(node->id());
            }
            for (auto *child_cl : cluster_ptr->clusters()) {
                p_cluster->add_child_cluster_ids(child_cl->id());
            }
        }

        for (const auto &edge_ptr : builder.edges()) {
            auto *p_edge = proto_graph.add_edges();
            p_edge->set_left_id(edge_ptr->left());
            p_edge->set_right_id(edge_ptr->right());
            p_edge->set_type(edge_ptr->type());
            p_edge->set_label(edge_ptr->label());
        }

        std::ofstream output(filename, std::ios::binary);
        if (!proto_graph.SerializeToOstream(&output)) {
            return 1;
        }
        return 0;
    }

    static void deserialize(gb::GraphBuilder &builder, const std::string& filepath) {
        gb_ser::Graph static_graph;
        std::ifstream static_file(filepath, std::ios::binary);
        
        if (!static_file) {
            throw std::runtime_error("Failed to open " + filepath);
        }
        
        if (!static_graph.ParseFromIstream(&static_file)) {
            throw std::runtime_error("Failed to parse protobuf data");
        }

        std::unordered_map<gb::IdT, const gb_ser::Node *> nodes;
        for (auto &pb_node : static_graph.nodes()) {
            
            uint64_t node_type = pb_node.type(); 
            gb::IdT node_id = pb_node.id();
            nodes[node_id] = &pb_node;
            gb::INode *node = nullptr;
            switch (node_type) {
                case gb::Instr: node = builder.create_node<gb::Instr>(node_id); break;
                case gb::Constant: node = builder.create_node<gb::Constant>(node_id); break;
                default: throw std::runtime_error("Got unknown node type: '" + std::to_string(node_type) + "' during serialization.");
            }
            node->label() = pb_node.label();
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
            gb::IdT cluster_id = pb_cluster.id();
            gb::IdT parent_id = pb_cluster.parent_id();
            gb::ICluster *cluster = builder.get_cluster(cluster_id);
            cluster->set_parent(builder.get_cluster(parent_id));
            for (gb::IdT node_id : pb_cluster.child_node_ids()) {
                builder.get_node(node_id)->set_parent(cluster);
            }
        }

    }
};

} // namespace gb