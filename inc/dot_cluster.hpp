#pragma once

#include <cassert>
#include "graph_builder.hpp"

namespace dot 
{

struct ClusterProperties {
    std::string color = "red";
    std::string fillcolor = "gray";
    std::string style = "filled";
    int penwidth = 2; 
    std::string fontcolor = "red";
    int fontsize = 20;
};

static inline const ClusterProperties DEFAULT_BB_CLUSTER_PROPERTIES = 
{
    .color = "red",
    .fillcolor = "gray",
    .style = "filled",
    .penwidth = 2,
    .fontcolor = "red",
    .fontsize = 20,
};

class BBCluster final : public ICluster {
    ClusterProperties properties_=DEFAULT_BB_CLUSTER_PROPERTIES;

    DotId id_;
    std::string label_;
    
    ICluster *parent_=nullptr;
    std::vector<INode *> children_;

public:
    BBCluster(DotId cluster_id, const std::string &label):
        id_(cluster_id), label_(label) {}
    
    DotId id() const override { return id_; }
    std::string label() const override { return label_; }

    void add_child(INode* node) override {
        assert(node);
    
        children_.push_back(node);
    }

    const ClusterProperties& properties() const override {
        return properties_;
    }

    ClusterProperties& properties() override {
        return properties_;
    }

    const ICluster* get_parent() const override {
        return parent_;
    }

    ICluster* get_parent() override {
        return parent_;
    }

    void set_parent(ICluster* parent) override {
        parent_ = parent;
    }

    const std::vector<INode*>& children() const override {
        return children_;
    }
};

}