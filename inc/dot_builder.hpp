#pragma once

#include "dot_interface.hpp"
#include "dot_node.hpp"
#include "dot_edge.hpp"

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

    DotId next_id_{0};

    std::unordered_map<DotId, std::unique_ptr<INode>> nodes_;
    std::unordered_map<DotId, std::unique_ptr<ICluster>> clusters_;
    std::vector<std::unique_ptr<IEdge>> edges_;

private:
    DotId alloc_id() { return ++next_id_; }

    template <typename EdgeT, typename... ArgT>
    EdgeT& create_edge_impl(Endpoint left, Endpoint right, ArgT&&... args) {
        static_assert(std::is_base_of<IEdge, EdgeT>::value,
                      "EdgeT must derive from IEdge");
        
        auto ptr = std::make_unique<EdgeT>(left, right, std::forward<ArgT>(args)...);
        EdgeT& ref = *ptr;
        edges_.push_back(std::move(ptr));
        return ref;
    }

public:
    DotBuilder() = default;


    template <typename ClusterT, typename ... ArgT>   
    ClusterT& create_cluster(ArgT&& ... args) {
        static_assert(std::is_base_of<ICluster, ClusterT>::value,
                      "ClusterT must derive from ICluster");
        
        DotId id = alloc_id();
        auto ptr = std::make_unique<ClusterT>(id, std::forward<ArgT>(args)...);
        ClusterT& ref = *ptr;
        clusters_.emplace(id, std::move(ptr));
        return ref;
    };

    template <typename ClusterT, typename... ArgT>
    ClusterT& create_cluster_with_parent(ICluster& parent, ArgT&&... args) {
        ClusterT& cluster = create_cluster<ClusterT>(std::forward<ArgT>(args)...);
        cluster.set_parent(parent);
        return cluster;
    }

    template <typename NodeT, typename... ArgT>
    NodeT& create_node(ArgT&&... args) {
        static_assert(std::is_base_of<INode, NodeT>::value,
                      "NodeT must derive from INode");
                    
        DotId id = alloc_id();
        auto ptr = std::make_unique<NodeT>(id, std::forward<ArgT>(args)...);
        NodeT& ref = *ptr;
        nodes_.emplace(id, std::move(ptr));
        return ref;
    }

    template <typename NodeT, typename... ArgT>
    NodeT& create_node_in_cluster(ICluster& cluster, ArgT&&... args) {
        static_assert(std::is_base_of<INode, NodeT>::value,
                      "NodeT must derive from INode");
                    
        DotId cid = cluster.id();
        if (clusters_.find(cid) == clusters_.end()) {
            throw std::runtime_error("cluster not owned by builder");
        }

        DotId id = alloc_id();
        auto ptr = std::make_unique<NodeT>(id, std::forward<ArgT>(args)...);
        NodeT& ref = *ptr;
        nodes_.emplace(id, std::move(ptr));
        clusters_[cid]->add_child(ptr);
        return ref;
    }
    
    template <typename EdgeT, typename... ArgT>
    EdgeT& create_edge(INode& left, INode& right, ArgT&&... args) {
        return create_edge_impl<EdgeT>(Endpoint::node(left.id()),
                                       Endpoint::node(right.id()),
                                       std::forward<ArgT>(args)...);
    }
    template <typename EdgeT, typename... ArgT>
    EdgeT& create_edge(ICluster& left, INode& right, ArgT&&... args) {
        return create_edge_impl<EdgeT>(Endpoint::cluster(left.id()),
                                       Endpoint::node(right.id()),
                                       std::forward<ArgT>(args)...);
    }
    template <typename EdgeT, typename... ArgT>
    EdgeT& create_edge(INode& left, ICluster& right, ArgT&&... args) {
        return create_edge_impl<EdgeT>(Endpoint::node(left.id()),
                                       Endpoint::cluster(right.id()),
                                       std::forward<ArgT>(args)...);
    }
    template <typename EdgeT, typename... ArgT>
    EdgeT& create_edge(ICluster& left, ICluster& right, ArgT&&... args) {
        return create_edge_impl<EdgeT>(Endpoint::cluster(left.id()),
                                       Endpoint::cluster(right.id()),
                                       std::forward<ArgT>(args)...);
    }
};

} // namespace dot

