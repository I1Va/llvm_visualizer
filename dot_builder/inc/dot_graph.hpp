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

    void apply_dynamic_info(const DynamicInfo &info) {
        max_bb_count_ = 0;
    
        for (auto [block_id, block_cnt] : info.bb_counts()) {
            auto it = clusters_.find(block_id);
            assert(it != clusters_.end());
            Cluster *cluster = it->second.get();
            assert(cluster && cluster->type() == gb::ClusterTypes::BB);
            cluster->set_use_count(block_cnt);
            max_bb_count_ = std::max(max_bb_count_, block_cnt);
        }

        for (auto [call_edge_id, call_edge_count] : info.call_edge_counts()) {
            Edge *edge = get_call_edge(call_edge_id.first, call_edge_id.second);
            assert(edge && edge->type() == gb::EdgeTypes::Call);
            edge->set_use_count(call_edge_count);
        }

        for (auto &[call_id, call_values] : info.call_values()) {
            auto it = nodes_.find(call_id);
            assert(it != nodes_.end());
            Node *node = it->second.get();
            assert(node && node->type() == gb::NodeTypes::Instr && node->label() == "call");
            for (int64_t value : call_values)
                node->add_call_value(value);
        }
    }

   void serialize_dot(std::ostream &stream) const {
        stream << "digraph "      << properties_.name    << " {\n";
        stream << "  rankdir=\""  << properties_.rankdir  << "\"\n";
        stream << "  splines=\""  << properties_.splines  << "\"\n";  
        stream << "  nodesep=\""  << properties_.nodesep  << "\"\n";       
        stream << "  ranksep=\""  << properties_.ranksep  << "\"\n";   
        stream << "  bgcolor=\""  << properties_.bgcolor  << "\"\n";      
        stream << "  fontname=\"" << properties_.fontname << "\"\n";   
        stream << "  fontsize=\"" << properties_.fontsize << "\"\n";   
        
        print_clusters(stream);

        const size_t indent = 2;
        
        for (auto &edge : edges_) edge->print(stream, indent);

        stream << "}\n";
    }
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
    ) const {
        const std::string indent_string(indent, ' ');
        clusters_.at(cluster_id)->print_open(stream, indent);

        if (cluster_graph.find(cluster_id) != cluster_graph.end()) {
            for (gb::IdT child_id : cluster_graph.at(cluster_id)) {
                print_cluster_recursive(stream, cluster_graph, child_id, indent + 2);
            }
        }
        clusters_.at(cluster_id)->print_close(stream, indent);
    }

    void print_clusters(std::ostream &stream) const {
        std::unordered_map<gb::IdT, std::vector<gb::IdT>> clusters_graph(clusters_.size());

        for (auto &[cluster_id, cluster] : clusters_) {
            gb::ICluster* parent = cluster->parent();
            if (parent) {
                clusters_graph[parent->id()].push_back(cluster_id);
                
            }
        }
        
        const size_t start_indent = 2; 
        for (auto &[cluster_id, cluster] : clusters_) {
            if (cluster->parent() == nullptr) {
                print_cluster_recursive(stream, clusters_graph, cluster_id, start_indent);
            }
        }
    }
};

} // namespace dot
