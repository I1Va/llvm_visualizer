#pragma once
#include <string>
#include <iostream>

#include "gb_node.hpp"
#include "gb_llvm_types.hpp"

namespace dot
{

class DotGraph;

struct ClusterProperties {
    std::string cluster_suffix = "";
    std::string color = "#000000";       
    std::string fillcolor = "#cccccc";   
    std::string style = "filled,rounded"; 
    int penwidth = 2; 
    std::string fontcolor = "#000000";   
    int fontsize = 16;
};   

static inline const ClusterProperties DEFAULT_BB_CLUSTER_PROPERTIES = 
{
    .cluster_suffix = "BasicBlock",
    .color = "#4f81bd",          
    .fillcolor = "#dce6f1",      
    .style = "filled,rounded",
    .penwidth = 2,
    .fontcolor = "#0b3d91",      
    .fontsize = 16,
};

static inline const ClusterProperties DEFAULT_F_CLUSTER_PROPERTIES = 
{
    .cluster_suffix = "Function",
    .color = "#c0504d",          
    .fillcolor = "#f2dcdb",      
    .style = "filled,rounded",
    .penwidth = 2,
    .fontcolor = "#801818",      
    .fontsize = 18,
};

class Cluster {
    gb::ICluster *underlying_;
    DotGraph *graph_;
    ClusterProperties properties_;
public:
    explicit Cluster(gb::ICluster *cluster, DotGraph *graph): underlying_(cluster), graph_(graph) {
        if (!underlying_) throw std::invalid_argument("Cannot wrap a null INode.");
        if (!graph_) throw std::invalid_argument("Nullptr DotGraph ptr passed."); 

        switch (cluster->type()) {
            case gb::ClusterTypes::BB: properties_ = DEFAULT_BB_CLUSTER_PROPERTIES; break;
            case gb::ClusterTypes::F:  properties_ = DEFAULT_F_CLUSTER_PROPERTIES; break;
            default: throw std::runtime_error("Got unknown cluster type: '" + std::to_string(cluster->type()) + "' in wrapper constructor.");
        }  
    }

    gb::IdT id() const { return underlying_->id(); }
    uint64_t type() const { return underlying_->type(); }
    const std::string &label() const { return underlying_->label(); }
    std::string &label() { return underlying_->label(); }

    gb::ICluster* parent() { return underlying_->parent(); }
    const gb::ICluster* parent() const { return underlying_->parent(); }
    void set_parent(gb::ICluster* parent) {
       underlying_->set_parent(parent);
    }

    const std::vector<gb::INode*>& nodes() const { return underlying_->nodes(); }
    std::vector<gb::INode*>& nodes() { return underlying_->nodes(); }

    const std::vector<gb::ICluster*>& clusters() const { return underlying_->clusters(); }
    std::vector<gb::ICluster*>& clusters() { return underlying_->clusters(); }

    void remove_cluster(gb::ICluster* cluster) {
        underlying_->remove_cluster(cluster);        
    }

    void remove_node(gb::INode* node) {
        underlying_->remove_node(node);
    }

    static std::string get_str_identifier(const gb::IdT id) {
        return "cluster" + std::to_string(id);
    }

    static std::string get_fict_node_str_id(gb::IdT id) {
        return "cluster_fictitious_node" + std::to_string(id);
    }

    void print_open(std::ostream &stream, const size_t indent) const; 
    void print_close(std::ostream &stream, const size_t indent) const;
};

} // namespace dot