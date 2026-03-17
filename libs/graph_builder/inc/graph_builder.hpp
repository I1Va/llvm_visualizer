#pragma once

#include <map>
#include <memory>

#include "gb_interface.hpp"
#include "gb_node.hpp"
#include "gb_edge.hpp"
#include "gb_cluster.hpp"

namespace gb
{

class GraphBuilder {
    std::map<IdT, std::unique_ptr<INode>> nodes_;
    std::map<IdT, std::unique_ptr<ICluster>> clusters_;
    std::map<std::pair<IdT, IdT>, std::unique_ptr<IEdge>> edges_;

public:
    bool is_edge(const std::pair<IdT, IdT> id) {
        return edges_.find(id) != edges_.end();
    }
    bool is_node(const IdT id) {
        return nodes_.find(id) != nodes_.end();
    }
    bool is_cluster(const IdT id) {
        return clusters_.find(id) != clusters_.end();
    }

    const std::map<IdT, std::unique_ptr<INode>>                 &nodes() const { return nodes_; };
    const std::map<IdT, std::unique_ptr<ICluster>>              &clusters() const { return clusters_; };
    const std::map<std::pair<IdT, IdT>, std::unique_ptr<IEdge>> &edges() const { return edges_; };
    std::map<IdT, std::unique_ptr<INode>>                       &nodes() { return nodes_; };
    std::map<IdT, std::unique_ptr<ICluster>>                    &clusters() { return clusters_; };
    std::map<std::pair<IdT, IdT>, std::unique_ptr<IEdge>>       &edges() { return edges_; };


private:
    template <uint64_t EdgeT>
    IEdge* create_edge_impl(const IdT left, const IdT right) {
        const std::pair<IdT, IdT> id(left, right);
        auto it = edges_.find(id);
        if (it != edges_.end()) {
            return it->second.get();
        }
    
        auto edge = std::make_unique<Edge>(left, right, EdgeT);
        IEdge* edge_ptr = edge.get();
        edges_.emplace(edge->id(), std::move(edge));
        return edge_ptr;
    }
public:
    template <uint64_t EdgeT>
    IEdge* create_edge(INode& left, INode& right) {
        return create_edge_impl<EdgeT>(left.id(), right.id());
    }

    template <uint64_t EdgeT>
    IEdge* create_edge(ICluster& left, INode& right) {
        return create_edge_impl<EdgeT>(left.id(), right.id());
    }
    template <uint64_t EdgeT>
    IEdge* create_edge(INode& left, ICluster& right) {
        return create_edge_impl<EdgeT>(left.id(), right.id());
    }
    template <uint64_t EdgeT>
    IEdge* create_edge(ICluster& left, ICluster& right) {
        return create_edge_impl<EdgeT>(left.id(), right.id());
    }

    GraphBuilder() = default;

    template <uint64_t ClusterT>
    ICluster* create_cluster(IdT id) {
        auto it = clusters_.find(id);
        if (it != clusters_.end()) {
            return it->second.get();
        }

        auto cluster = std::make_unique<Cluster>(id, ClusterT);
        ICluster* cluster_ptr = cluster.get();
        clusters_.emplace(id, std::move(cluster));
        return cluster_ptr;
    }

    template <uint64_t NodeT>
    INode* create_node(IdT id) {
        auto it = nodes_.find(id);
        if (it != nodes_.end()) {
            return it->second.get();
        }
    
        auto node = std::make_unique<Node>(id, NodeT);
        INode* node_ptr = node.get();
        nodes_.emplace(id, std::move(node));
        return node_ptr;
    }

    ICluster *get_cluster(const IdT id) {
        if (clusters_.find(id) == clusters_.end()) {
            return nullptr;
        }
        return clusters_.find(id)->second.get();
    }

    INode *get_node(const IdT id) {
        if (nodes_.find(id) == nodes_.end()) {
            return nullptr;
        }
        return nodes_.find(id)->second.get();
    }
};

} // namespace dot