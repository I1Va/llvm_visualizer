#pragma once
#include <memory>
#include <iostream>
#include <map>
#include <unordered_map>

#include "graph_builder.hpp"
#include "dot_node.hpp"
#include "dot_edge.hpp"
#include "dot_cluster.hpp"
#include "dynamic_info.hpp"

namespace dot
{

class DotGraph {
public:
    struct Properties {
        std::string name = "LLVMGraph";    
        std::string rankdir = "TB";        
        std::string splines = "true";      
        double nodesep = 0.8;              
        double ranksep = 1.2;              
        std::string bgcolor = "#f8f9fa";   
        std::string fontname = "Helvetica"; 
        int fontsize = 14;   
        
        Properties() = default;
    };
private:
    Properties properties_;
    std::map<gb::IdT, std::unique_ptr<Node>> nodes_;                   
    std::map<gb::IdT, std::unique_ptr<Cluster>> clusters_;                 
    std::vector<std::unique_ptr<Edge>> edges_;

    uint64_t max_bb_count_ = 1;

public:
    explicit DotGraph(gb::GraphBuilder &builder) {
        for (auto &[id, cluster] : builder.clusters()) {
            clusters_[id] = std::make_unique<Cluster>(cluster.get(), this);
        }
        for (auto &[id, node] : builder.nodes()) {
            nodes_[id] = std::make_unique<Node>(node.get(), this);
        }
        for (auto &edge : builder.edges()) {
            edges_.push_back(std::move(std::make_unique<Edge>(edge.get(), this)));
        }
    }

    std::map<gb::IdT, std::unique_ptr<Node>>    &nodes() { return nodes_; };
    std::map<gb::IdT, std::unique_ptr<Cluster>> &clusters() { return clusters_; };
    std::vector<std::unique_ptr<Edge>>          &edges() { return edges_; };
    
    uint64_t max_bb_count() const { return max_bb_count_; }

    Node *get_node(const gb::IdT id) {
        auto it = nodes_.find(id);
        if (it != nodes_.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    Cluster *get_cluster(const gb::IdT id) {
        auto it = clusters_.find(id);
        if (it != clusters_.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    void apply_dynamic_info(const DynamicInfo &info);

    void serialize_dot(std::ostream &stream) const;

private:
    Edge *get_call_edge(const gb::IdT left, const gb::IdT right) {
        for (auto &edge : edges_) {
            if (edge->type() == gb::EdgeTypes::Call && 
                edge->left() == left && edge->right() == right) {
                    return edge.get();
                }
        }
        return nullptr;
    }

    void print_cluster_recursive (
        std::ostream &stream,
        const std::unordered_map<gb::IdT, std::vector<gb::IdT>>& cluster_graph,
        const gb::IdT cluster_id,
        const size_t indent
    ) const;

    void print_clusters(std::ostream &stream) const;
};

} // namespace dot
