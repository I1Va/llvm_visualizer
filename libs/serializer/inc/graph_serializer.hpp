#pragma once

#include <google/protobuf/stubs/common.h>
#include <fstream>

#include "graph_builder.hpp"
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
};

} // namespace gb