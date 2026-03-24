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
    std::vector<std::unique_ptr<IEdge>> edges_;

public:
    IEdge* get_edge(const IdT left, const IdT right, const uint64_t edge_type) {
        for (auto &edge : edges_) {
            if (edge->left() == left && edge->right() == right && edge->type() == edge_type) {
                return edge.get();
            }
        }

        return nullptr;
    } 

    INode *get_node(const IdT id) {
        auto it = nodes_.find(id);
        if (it != nodes_.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    ICluster *get_cluster(const IdT id) {
        auto it = clusters_.find(id);
        if (it != clusters_.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    const std::map<IdT, std::unique_ptr<INode>>                 &nodes() const { return nodes_; };
    const std::map<IdT, std::unique_ptr<ICluster>>              &clusters() const { return clusters_; };
    const std::vector<std::unique_ptr<IEdge>>                   &edges() const { return edges_; };
    std::map<IdT, std::unique_ptr<INode>>                       &nodes() { return nodes_; };
    std::map<IdT, std::unique_ptr<ICluster>>                    &clusters() { return clusters_; };
    std::vector<std::unique_ptr<IEdge>>                         &edges() { return edges_; };

private:
    template <uint64_t EdgeT>
    IEdge* create_edge_impl(const IdT left, const IdT right) {
        IEdge *contained_edge = get_edge(left, right, EdgeT);
        if (contained_edge) return contained_edge;
    
        auto edge = std::make_unique<Edge>(left, right, EdgeT);
        IEdge* edge_ptr = edge.get();
        edges_.push_back(std::move(edge));
        return edge_ptr;
    }

public:
    template <uint64_t EdgeT>
    IEdge* create_edge(std::pair<IdT, IdT> id) {
        return create_edge_impl<EdgeT>(id.first, id.second);
    }

    template <uint64_t EdgeT>
    IEdge* create_edge(IdT left, IdT right) {
        return create_edge_impl<EdgeT>(left, right);
    }

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
};

} // namespace gb