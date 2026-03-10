#pragma once

#include <cassert>
#include "dot_interface.hpp"

namespace dot 
{

struct ClusterProperties {
    std::string cluster_suffix = "";
    std::string color = "red";
    std::string fillcolor = "gray";
    std::string style = "filled";
    int penwidth = 2; 
    std::string fontcolor = "red";
    int fontsize = 20;

    ClusterProperties() = default;
};

static inline const ClusterProperties DEFAULT_BB_CLUSTER_PROPERTIES = 
{
    .cluster_suffix = "BasicBlock",
    .color = "red",
    .fillcolor = "gray",
    .style = "filled",
    .penwidth = 2,
    .fontcolor = "red",
    .fontsize = 20,
};

static inline const ClusterProperties DEFAULT_F_CLUSTER_PROPERTIES = 
{
    .cluster_suffix = "Function",
    .color = "green",
    .fillcolor = "gray",
    .style = "filled",
    .penwidth = 2,
    .fontcolor = "red",
    .fontsize = 20,
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

    std::string get_cluster_name() const override {
        return "cluster_" + std::to_string(id_);
    }

    void print_open(std::ostream &stream, const size_t indent) const override {
        const std::string indent_string(indent, ' ');
        stream << indent_string << "subgraph "      << get_cluster_name()    << " {" << "// " << properties_.cluster_suffix << "\n";
        stream << indent_string << "  label =\""    << label_                << "\"\n";
        stream << indent_string << "  color =\""    << properties_.color     << "\"\n"; 
        stream << indent_string << "  style =\""    << properties_.style     << "\"\n";
        stream << indent_string << "  penwidth =\"" << properties_.penwidth  << "\"\n";
        stream << indent_string << "  fontcolor=\"" << properties_.fontcolor << "\"\n";     
        stream << indent_string << "  fontsize=\""  << properties_.fontsize  << "\"\n";
        
        for (const INode *child : children_) {
            child->print(stream, indent);
        }
    }

    void print_close(std::ostream &stream, const size_t indent) const override {
        const std::string indent_string(indent, ' ');
        stream << indent_string << "}\n";
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