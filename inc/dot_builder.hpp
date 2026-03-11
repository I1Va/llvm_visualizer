#pragma once

#include <sstream>
#include <fstream>

#include "dot_interface.hpp"
#include "dot_node.hpp"
#include "dot_edge.hpp"
#include "dot_cluster.hpp"

#include "llvm/Support/raw_ostream.h"

namespace dot
{

struct GraphProperties {
    std::string name = "LLVMGraph";    
    std::string rankdir = "TB";        
    std::string splines = "true";      
    double nodesep = 0.8;              
    double ranksep = 1.2;              
    std::string bgcolor = "#f8f9fa";   
    std::string fontname = "Helvetica"; 
    int fontsize = 14;                 
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

    void print_cluster_recursive (
        std::ostream &stream,
        const std::unordered_map<DotId, std::vector<DotId>>& cluster_graph,
        const DotId cluster_id,
        const size_t indent
    ) const {
        const std::string indent_string(indent, ' ');
        clusters_.at(cluster_id)->print_open(stream, indent);

        if (cluster_graph.find(cluster_id) != cluster_graph.end()) {
            for (DotId child_id : cluster_graph.at(cluster_id)) {
                print_cluster_recursive(stream, cluster_graph, child_id, indent + 2);
            }
        }
        clusters_.at(cluster_id)->print_close(stream, indent);
    }

    void print_clusters(std::ostream &stream) const {
        std::unordered_map<DotId, std::vector<DotId>> clusters_graph(clusters_.size());

        for (auto &[cluster_id, cluster] : clusters_) {
            ICluster* parent = cluster->parent();
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
            llvm::errs() << "error\n";
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
            clusters_[cid]->add_child(node_ptr);
        }
        
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

    ICluster *get_cluster_by_id(const DotId id) const {
        if (clusters_.find(id) == clusters_.end()) {
            return nullptr;
        }
        return clusters_.find(id)->second.get();
    }

    INode *get_node_by_id(const DotId id) const {
        if (nodes_.find(id) == nodes_.end()) {
            return nullptr;
        }
        return nodes_.find(id)->second.get();
    }

    bool node_id_exist(const DotId id) const {
        return nodes_.find(id) != nodes_.end();
    }

    bool cluster_id_exist(const DotId id) const {
        return clusters_.find(id) != clusters_.end();
    }

    void serialize_dot(std::ostream &stream) const {
        stream << "digraph "      << properties.name    << " {\n";
        stream << "  rankdir=\""  << properties.rankdir  << "\"\n";
        stream << "  splines=\""  << properties.splines  << "\"\n";  
        stream << "  nodesep=\""  << properties.nodesep  << "\"\n";       
        stream << "  ranksep=\""  << properties.ranksep  << "\"\n";   
        stream << "  bgcolor=\""  << properties.bgcolor  << "\"\n";      
        stream << "  fontname=\"" << properties.fontname << "\"\n";   
        stream << "  fontsize=\"" << properties.fontsize << "\"\n";   
        
        print_clusters(stream);

        const size_t indent = 2;
        
        for (auto &edge : edges_) edge->print(stream, clusters_, indent);

        stream << "}\n";
    }
};

} // namespace dot