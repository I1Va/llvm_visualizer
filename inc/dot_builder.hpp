#pragma once

#include "dot_interface.hpp"
#include "dot_node.hpp"
#include "dot_edge.hpp"
#include "dot_cluster.hpp"
#include <iostream>
namespace dot
{

struct GraphProperties {
    std::string_view name = "G";
    std::string_view rankdir = "TD";
    std::string_view splines = "polyline"; 
    double nodesep = 1.0; 
    double ranksep = 1.5;
};

class DotBuilder {
    GraphProperties properties;

    std::unordered_map<DotId, std::unique_ptr<INode>> nodes_;
    std::unordered_map<DotId, std::unique_ptr<ICluster>> clusters_;
    std::vector<std::unique_ptr<IEdge>> edges_;

private:
    template <typename EdgeT, typename... ArgT>
    EdgeT* create_edge_impl(Endpoint left, Endpoint right, ArgT&&... args) {
        static_assert(std::is_base_of<IEdge, EdgeT>::value,
                      "EdgeT must derive from IEdge");
                            
        auto edge = std::make_unique<EdgeT>(left, right, std::forward<ArgT>(args)...);
        EdgeT* edge_ptr = edge.get();
        edges_.push_back(std::move(edge));
        
        return edge_ptr;
    }

public:
    DotBuilder() = default;

    template <typename ClusterT, typename ... ArgT>
    ClusterT* create_cluster(DotId id, ArgT&& ... args) {
        static_assert(std::is_base_of<ICluster, ClusterT>::value, "ClusterT must derive from ICluster");

        auto it = clusters_.find(id);
        if (it != clusters_.end()) {
            return static_cast<ClusterT*>(it->second.get());
        }

        auto cluster = std::make_unique<ClusterT>(id, std::forward<ArgT>(args)...);
        ClusterT* cluster_ptr = cluster.get();
        clusters_.emplace(id, std::move(cluster));
        return cluster_ptr;
    }

    template <typename ClusterT, typename... ArgT>
    ClusterT* create_cluster_with_parent(DotId id, ICluster& parent, ArgT&&... args) {
        ClusterT* cluster = create_cluster<ClusterT>(id, std::forward<ArgT>(args)...);
        cluster->set_parent(&parent);
        return cluster; 
    }

    template <typename NodeT, typename... ArgT>
    NodeT* create_node(DotId id, ArgT&&... args) {
        static_assert(std::is_base_of<INode, NodeT>::value,
                      "NodeT must derive from INode");
               
        auto it = nodes_.find(id);
        if (it != nodes_.end()) {
            return static_cast<NodeT*>(it->second.get());
        }
    
        auto node = std::make_unique<NodeT>(id, std::forward<ArgT>(args)...);
        NodeT* node_ptr = node.get();
        nodes_.emplace(id, std::move(node));
        return node_ptr;
    }

    template <typename NodeT, typename... ArgT>
    NodeT* create_node_in_cluster(DotId id, ICluster& cluster, ArgT&&... args) {
        static_assert(std::is_base_of<INode, NodeT>::value,
                      "NodeT must derive from INode");
        DotId cid = cluster.id();
        if (clusters_.find(cid) == clusters_.end()) {
            throw std::runtime_error("cluster not owned by builder");
        }

        auto it = nodes_.find(id);
        NodeT* node_ptr = nullptr;
        if (it != nodes_.end()) {
            node_ptr = static_cast<NodeT*>(it->second.get());
        } else {
            auto node = std::make_unique<NodeT>(id, std::forward<ArgT>(args)...);
            node_ptr = node.get();
            nodes_.emplace(id, std::move(node));
        }

        clusters_[cid]->add_child(node_ptr);
        return node_ptr;
    }
    
    template <typename EdgeT, typename... ArgT>
    EdgeT* create_edge(INode& left, INode& right, ArgT&&... args) {
        return create_edge_impl<EdgeT>(Endpoint::node(left.id()),
                                       Endpoint::node(right.id()),
                                       std::forward<ArgT>(args)...);
    }
    template <typename EdgeT, typename... ArgT>
    EdgeT* create_edge(ICluster& left, INode& right, ArgT&&... args) {
        return create_edge_impl<EdgeT>(Endpoint::cluster(left.id()),
                                       Endpoint::node(right.id()),
                                       std::forward<ArgT>(args)...);
    }
    template <typename EdgeT, typename... ArgT>
    EdgeT* create_edge(INode& left, ICluster& right, ArgT&&... args) {
        return create_edge_impl<EdgeT>(Endpoint::node(left.id()),
                                       Endpoint::cluster(right.id()),
                                       std::forward<ArgT>(args)...);
    }
    template <typename EdgeT, typename... ArgT>
    EdgeT* create_edge(ICluster& left, ICluster& right, ArgT&&... args) {
        return create_edge_impl<EdgeT>(Endpoint::cluster(left.id()),
                                       Endpoint::cluster(right.id()),
                                       std::forward<ArgT>(args)...);
    }
};

} // namespace dot

