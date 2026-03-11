#pragma once

#include <cassert>
#include "dot_interface.hpp"

namespace dot 
{

struct ClusterProperties {
    std::string cluster_suffix = "";
    std::string color = "#000000";       
    std::string fillcolor = "#cccccc";   
    std::string style = "filled,rounded"; 
    int penwidth = 2; 
    std::string fontcolor = "#000000";   
    int fontsize = 16;

    ClusterProperties() = default;
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

class Cluster : public ICluster {
protected:
    ClusterProperties properties_;

    DotId id_;
    std::string label_;
    
    ICluster *parent_=nullptr;
    std::vector<INode *> children_;
public:
    Cluster(DotId cluster_id, const std::string& label):
        id_(cluster_id), label_(label) {}
    
    DotId id() const override { return id_; }
    std::string_view label() const override { return label_; }

    const ClusterProperties& properties() const override {
        return properties_;
    }

    ClusterProperties& properties() override {
        return properties_;
    }

    const ICluster* parent() const override {
        return parent_;
    }

    void add_child(INode* node) override {
        assert(node);
    
        children_.push_back(node);
    }

    ICluster* parent() override {
        return parent_;
    }

    void set_parent(ICluster* parent) override {
        parent_ = parent;
    }

    const std::vector<INode*>& children() const override {
        return children_;
    }

    void print_open(std::ostream &stream, const size_t indent) const override {
        const std::string indent_string(indent, ' ');
        stream << indent_string << "subgraph "       << get_str_identifier(id_) << " {" << "// " << properties_.cluster_suffix << "\n";
        stream << indent_string << "  label =\""     << label_                  << "\"\n";
        stream << indent_string << "  fillcolor =\"" << properties_.fillcolor   << "\"\n"; 
        stream << indent_string << "  color =\""     << properties_.color       << "\"\n"; 
        stream << indent_string << "  style =\""     << properties_.style       << "\"\n";
        stream << indent_string << "  penwidth =\""  << properties_.penwidth    << "\"\n";
        stream << indent_string << "  fontcolor=\""  << properties_.fontcolor   << "\"\n";     
        stream << indent_string << "  fontsize=\""   << properties_.fontsize    << "\"\n";
        
        for (const INode *child : children_) {
            child->print(stream, indent + 2);
        }
        if (children_.empty()) {
            const std::string indent_string(indent + 2, ' ');
            stream << indent_string << get_fict_node_str_id() << "[label=\"\", shape=\"point\"]\n";
        }
    }

    void print_close(std::ostream &stream, const size_t indent) const override {
        const std::string indent_string(indent, ' ');
        stream << indent_string << "}\n";
    }

    std::string get_fict_node_str_id() const override {
        return "cluster_fictitious_node" + std::to_string(id_);
    }

};

class BBCluster final : public Cluster {
public:
    BBCluster(DotId cluster_id, const std::string& label) : 
        Cluster(cluster_id, label) {
            properties_ = DEFAULT_BB_CLUSTER_PROPERTIES;
        }
};

class FCluster final : public Cluster {
public:
    FCluster(DotId cluster_id, const std::string& label) : 
        Cluster(cluster_id, label) {
            properties_ = DEFAULT_F_CLUSTER_PROPERTIES;
        }
};

} // namespace dot