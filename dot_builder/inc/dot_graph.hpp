#pragma once
#include <memory>
#include <iostream>
#include <map>
#include <unordered_map>

#include "graph_builder.hpp"
#include "dot_node.hpp"
#include "dot_edge.hpp"
#include "dot_cluster.hpp"

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
    std::map<std::pair<gb::IdT, gb::IdT>, std::unique_ptr<Edge>> edges_;
        
public:
    explicit DotGraph(gb::GraphBuilder &builder) {
        for (auto &[id, cluster] : builder.clusters()) {
            clusters_[id] = std::make_unique<Cluster>(cluster.get(), this);
        }
        for (auto &[id, node] : builder.nodes()) {
            nodes_[id] = std::make_unique<Node>(node.get(), this);
        }
        for (auto &[id, edge] : builder.edges()) {
            edges_[id] = std::make_unique<Edge>(edge.get(), this);
        }
    }

    bool is_edge(const std::pair<gb::IdT, gb::IdT> id) {
        return edges_.find(id) != edges_.end();
    }
    bool is_node(const gb::IdT id) {
        return nodes_.find(id) != nodes_.end();
    }
    bool is_cluster(const gb::IdT id) {
        return clusters_.find(id) != clusters_.end();
    }

   std::map<gb::IdT, std::unique_ptr<Node>>                     &nodes() { return nodes_; };
   std::map<gb::IdT, std::unique_ptr<Cluster>>                  &clusters() { return clusters_; };
   std::map<std::pair<gb::IdT, gb::IdT>, std::unique_ptr<Edge>> &edges() { return edges_; };


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
        
        for (auto &[id, edge] : edges_) edge->print(stream, indent);

        stream << "}\n";
    }
private:
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
